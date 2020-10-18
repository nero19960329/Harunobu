#include <harunobu/integrator/render_utils.h>

HARUNOBU_NAMESPACE_BEGIN

inline real vec3_max_elem(const vec3 &v) {
    return std::max(std::max(v.x, v.y), v.z);
}

inline real power_heuristic(real a, real b) { return a * a / (a * a + b * b); }

sptr<SampleInfo>
RenderUtils::light_sample(sptr<Intersect> intersect,
                          const std::vector<sptr<SampleInfo>> &sinfo_vec) {
    if (sinfo_vec.size() == 1) {
        return sinfo_vec[0];
    }

    size_t n = sinfo_vec.size();
    int idx = rng.random_idx(0, n - 1);
    auto sinfo = sinfo_vec[idx];
    sinfo->pdf /= n;
    return sinfo;

    /*size_t n = sinfo_vec.size();
    std::vector<real> pdf(n);
    real sum = 0.;
    for (size_t i = 0; i < n; ++i) {
        real pdf_light;
        pdf[i] = vec3_max_elem(RenderUtils::get_direct_radiance_light_sampling(
            intersect, sinfo_vec[i], pdf_light));
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
    return sinfo;*/
}

vec3 RenderUtils::get_direct_radiance_light_sampling(
    sptr<Intersect> intersect, sptr<SampleInfo> light_sinfo, real &pdf_light) {
    vec3 x_prime = light_sinfo->pos;
    vec3 x = intersect->pos;
    vec3 wi = glm::normalize(x_prime - x);
    vec3 wo = intersect->omega;

    sptr<LocalInfo> linfo = std::make_shared<LocalInfo>(
        wi, wo, intersect->normal, intersect->prim->material->is_two_sided);
    real cos_theta_i = linfo->normal_dot(linfo->wi);
    real cos_theta_o = glm::dot(light_sinfo->normal, -wi);
    pdf_light = light_sinfo->pdf;
    if (cos_theta_i <= 0.0 || cos_theta_o <= 0.0) {
        return vec3(0, 0, 0);
    }

    vec3 radiance = light_sinfo->prim->emit_radiance;
    vec3 f = intersect->prim->material->f(linfo);
    return radiance * f * cos_theta_i / pdf_light;
}

vec3 RenderUtils::get_direct_radiance(std::vector<sptr<PrimitiveBase>> lights,
                                      sptr<Intersect> intersect,
                                      sptr<ObjectsBase> objects) {
    vec3 direct_radiance(0, 0, 0);
    real pdf_light, pdf_bsdf;

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
    vec3 wi_light = glm::normalize(x_prime - x);
    vec3 wo = intersect->omega;

    Ray light_test_ray(x, wi_light);
    light_test_ray.pos = light_test_ray.step(eps);
    bool is_intersect_light;
    auto intersect_light =
        objects->ray_intersect(light_test_ray, is_intersect_light);
    if (is_intersect_light &&
        std::abs(1. - glm::length(intersect_light->pos - x) /
                          glm::length(x_prime - x)) < eps) {
        light_radiance = get_direct_radiance_light_sampling(
            intersect, light_sinfo, pdf_light);

        sptr<LocalInfo> linfo = std::make_shared<LocalInfo>(
            wi_light, wo, intersect->normal,
            intersect->prim->material->is_two_sided);
        pdf_bsdf = intersect->prim->material->pdf(linfo);

        light_radiance *= power_heuristic(pdf_light, pdf_bsdf);
    }

    // bsdf sampling
    sptr<LocalInfo> linfo = std::make_shared<LocalInfo>(
        vec3(), wo, intersect->normal, intersect->prim->material->is_two_sided);
    intersect->prim->material->sample(linfo);
    pdf_bsdf = intersect->prim->material->pdf(linfo);
    vec3 wi_bsdf = linfo->to_world(linfo->wi);

    light_test_ray = Ray(x, wi_bsdf);
    light_test_ray.pos = light_test_ray.step(eps);
    intersect_light =
        objects->ray_intersect(light_test_ray, is_intersect_light);
    vec3 bsdf_radiance(0, 0, 0);
    real cos_theta_i =
        std::max(linfo->normal_dot(linfo->wi), static_cast<real>(0.));
    if (is_intersect_light &&
        glm::length(intersect_light->prim->emit_radiance) > 0.0) {
        vec3 emit_radiance = intersect_light->prim->emit_radiance;
        if (glm::dot(intersect_light->normal, -wi_bsdf) <= 0.0) {
            bsdf_radiance = vec3(0, 0, 0);
        } else {
            bsdf_radiance = emit_radiance *
                            intersect->prim->material->f(linfo) * cos_theta_i /
                            pdf_bsdf;
        }
        
        pdf_light = intersect_light->geo->light_sample_pdf(x, intersect_light->pos,
                                                     intersect_light->normal);
        pdf_light /= lights.size();
        pdf_light /= intersect->prim->geos.size();
        bsdf_radiance *= power_heuristic(pdf_bsdf, pdf_light);
    }

    direct_radiance = light_radiance + bsdf_radiance;
    return direct_radiance;
}

HARUNOBU_NAMESPACE_END