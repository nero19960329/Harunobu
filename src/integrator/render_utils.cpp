#include <harunobu/integrator/render_utils.h>

HARUNOBU_NAMESPACE_BEGIN

inline real vec3_max_elem(const vec3 &v) {
    return std::max(std::max(v.x, v.y), v.z);
}

sptr<SampleInfo>
RenderUtils::light_sample(sptr<Intersect> intersect,
                          const std::vector<sptr<SampleInfo>> &sinfo_vec) {
    if (sinfo_vec.size() == 1) {
        return sinfo_vec[0];
    }

    size_t n = sinfo_vec.size();
    std::vector<real> pdf(n);
    real sum = 0.;
    for (size_t i = 0; i < n; ++i) {
        pdf[i] = vec3_max_elem(RenderUtils::get_direct_radiance_wo_shadow_test(
            intersect, sinfo_vec[i]));
        sum += pdf[i];
    }

    if (sum != 0.0) {
        for (auto &w : pdf) {
            w /= sum;
        }
    } else {
        real w_avg = 1.0 / n;
        for (auto &w : pdf) {
            w = w_avg;
        }
    }

    std::vector<real> cdf(n + 1);
    cdf[0] = 0.;
    for (size_t i = 1; i <= n; ++i) {
        cdf[i] = pdf[i - 1] + cdf[i - 1];
    }

    real random = rng.random_real();
    size_t idx;
    for (idx = 1; idx <= n; ++idx) {
        if (random <= cdf[idx]) {
            break;
        }
    }

    auto sinfo = sinfo_vec[idx - 1];
    sinfo->pdf *= pdf[idx - 1];
    return sinfo;
}

vec3 RenderUtils::get_direct_radiance_wo_shadow_test(
    sptr<Intersect> intersect, sptr<SampleInfo> light_sinfo) {
    vec3 x_prime = light_sinfo->pos;
    vec3 x = intersect->pos;
    vec3 wi = glm::normalize(x_prime - x);
    vec3 wo = intersect->omega;

    sptr<LocalInfo> linfo = std::make_shared<LocalInfo>(
        wi, wo, intersect->normal, intersect->prim->material->is_two_sided);
    real cos_theta_i = linfo->normal_dot(linfo->wi);
    real cos_theta_o = glm::dot(light_sinfo->normal, -wi);
    if (cos_theta_i <= 0.0 || cos_theta_o <= 0.0) {
        return vec3(0, 0, 0);
    }

    vec3 radiance = light_sinfo->prim->emit_radiance;
    vec3 f = intersect->prim->material->f(linfo);
    real G = cos_theta_i * cos_theta_o / glm::length2(x - x_prime);

    return radiance * f * G / light_sinfo->pdf;
}

vec3 RenderUtils::get_direct_radiance(std::vector<sptr<PrimitiveBase>> lights,
                                      sptr<Intersect> intersect,
                                      sptr<ObjectsBase> objects) {
    vec3 direct_radiance(0, 0, 0);

    // light sampling
    int n = lights.size();
    std::vector<sptr<SampleInfo>> sinfo_vec(n);
    for (size_t i = 0; i < n; ++i) {
        auto sinfo = lights[i]->light_sample(intersect);
        sinfo_vec[i] = sinfo;
    }
    auto light_sinfo = RenderUtils::light_sample(intersect, sinfo_vec);

    vec3 light_radiance(0, 0, 0);
    vec3 x_prime = light_sinfo->pos;
    vec3 x = intersect->pos;
    vec3 wi = glm::normalize(x_prime - x);
    vec3 wo = intersect->omega;

    Ray light_test_ray(x, wi);
    light_test_ray.pos = light_test_ray.step(eps);
    bool is_intersect_light;
    auto intersect_light =
        objects->ray_intersect(light_test_ray, is_intersect_light);
    if (is_intersect_light &&
        std::abs(1. - glm::length(intersect_light->pos - x) /
                          glm::length(x_prime - x)) < eps) {
        light_radiance =
            get_direct_radiance_wo_shadow_test(intersect, light_sinfo);
    }

    // bsdf sampling

    direct_radiance = light_radiance;
    return direct_radiance;
}

HARUNOBU_NAMESPACE_END