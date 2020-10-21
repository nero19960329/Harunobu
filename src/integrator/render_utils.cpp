#include <harunobu/integrator/render_utils.h>

HARUNOBU_NAMESPACE_BEGIN

inline real vec3_max_elem(const vec3 &v) {
    return std::max(std::max(v.x, v.y), v.z);
}

inline real power_heuristic(real a, real b) { return a * a / (a * a + b * b); }

sptr<SampleInfo>
RenderUtils::light_sample(sptr<Intersect> intersect,
                          const std::vector<sptr<SampleInfo>> &sinfo_vec,
                          PMF &pmf) {
    size_t n = sinfo_vec.size();
    std::vector<real> pdf(n);
    for (size_t i = 0; i < n; ++i) {
        pdf[i] = sinfo_vec[i]->prim->light_sampling_weight;
    }
    pmf.build(pdf);
    auto idx = pmf.sample();
    auto sinfo = sinfo_vec[idx];
    sinfo->pdf *= pmf.get_pdf(idx);
    return sinfo;
}

vec3 RenderUtils::get_direct_radiance(std::vector<sptr<PrimitiveBase>> lights,
                                      sptr<Intersect> intersect,
                                      sptr<ObjectsBase> objects,
                                      int light_sample_num,
                                      int bsdf_sample_num) {
    vec3 direct_radiance(0, 0, 0);
    vec3 light_radiance(0, 0, 0), bsdf_radiance(0, 0, 0);

    size_t n = lights.size();
    PMF prims_pmf(n);
    std::vector<PMF> geos_pmf;
    for (size_t i = 0; i < n; ++i) {
        geos_pmf.push_back(PMF(lights[i]->geos.size()));
    }

    real light_prob = light_sample_num / static_cast<real>(light_sample_num + bsdf_sample_num);
    real bsdf_prob = bsdf_sample_num / static_cast<real>(light_sample_num + bsdf_sample_num);

    // light sampling
    for (int light_sample = 0; light_sample < light_sample_num; ++light_sample) {
        std::vector<sptr<SampleInfo>> sinfo_vec(n);
        for (size_t i = 0; i < n; ++i) {
            auto sinfo = lights[i]->light_sample(intersect, geos_pmf[i]);
            sinfo_vec[i] = sinfo;
        }
        auto light_sinfo =
            RenderUtils::light_sample(intersect, sinfo_vec, prims_pmf);

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
            sptr<LocalInfo> linfo = std::make_shared<LocalInfo>(
                wi, wo, intersect->normal, intersect->prim->material->is_two_sided);
            real cos_theta_i = linfo->normal_dot(linfo->wi);
            real cos_theta_o = glm::dot(light_sinfo->normal, -wi);
            if (cos_theta_i <= 0.0 || cos_theta_o <= 0.0) {
                return vec3(0, 0, 0);
            }

            vec3 emit_radiance = light_sinfo->prim->emit_radiance;
            vec3 f = intersect->prim->material->f(linfo);
            light_radiance = emit_radiance * f * cos_theta_i / light_sinfo->pdf;

            real pdf_light = light_sinfo->pdf;
            linfo = std::make_shared<LocalInfo>(
                wi, wo, intersect->normal,
                intersect->prim->material->is_two_sided);
            real pdf_bsdf = intersect->prim->material->pdf(linfo);

            light_radiance *= power_heuristic(pdf_light * light_prob, pdf_bsdf * bsdf_prob) / static_cast<real>(light_sample_num);
        }
    }

    // bsdf sampling
    for (int bsdf_sample = 0; bsdf_sample < bsdf_sample_num; ++bsdf_sample) {
        vec3 x = intersect->pos;
        vec3 wo = intersect->omega;

        sptr<LocalInfo> linfo = std::make_shared<LocalInfo>(
            vec3(), wo, intersect->normal, intersect->prim->material->is_two_sided);
        intersect->prim->material->sample(linfo);
        real pdf_bsdf = intersect->prim->material->pdf(linfo);
        vec3 wi = linfo->to_world(linfo->wi);

        Ray light_test_ray(x, wi);
        light_test_ray.pos = light_test_ray.step(eps);
        bool is_intersect_light;
        auto intersect_light =
            objects->ray_intersect(light_test_ray, is_intersect_light);
        real cos_theta_i =
            std::max(linfo->normal_dot(linfo->wi), static_cast<real>(0.));
        if (is_intersect_light &&
            glm::length(intersect_light->prim->emit_radiance) > 0.0) {
            vec3 emit_radiance = intersect_light->prim->emit_radiance;
            if (glm::dot(intersect_light->normal, -wi) > 0.0) {
                bsdf_radiance = emit_radiance *
                                intersect->prim->material->f(linfo) * cos_theta_i /
                                pdf_bsdf;
            }

            real pdf_light = intersect_light->geo->light_sample_pdf(
                x, intersect_light->pos, intersect_light->normal);
            pdf_light *= geos_pmf[intersect_light->prim->idx].get_pdf(
                intersect_light->geo->idx);
            pdf_light *= prims_pmf.get_pdf(intersect_light->prim->idx);
            bsdf_radiance *= power_heuristic(pdf_bsdf * bsdf_prob, pdf_light * light_prob) / static_cast<real>(bsdf_sample_num);
        }
    }

    direct_radiance = light_radiance + bsdf_radiance;
    return direct_radiance;
}

HARUNOBU_NAMESPACE_END