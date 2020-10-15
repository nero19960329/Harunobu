#include <harunobu/integrator/render_utils.h>

HARUNOBU_NAMESPACE_BEGIN

vec3 RenderUtils::get_direct_radiance(std::vector<sptr<PrimitiveBase>> lights,
                                      sptr<Intersect> intersect,
                                      sptr<ObjectsBase> objects) {
    vec3 direct_radiance(0, 0, 0);

    for (const auto &light : lights) {
        auto light_sinfo = light->random_sample();
        vec3 x_prime = light_sinfo->pos;
        vec3 x = intersect->pos;
        vec3 wi = glm::normalize(x_prime - x);
        vec3 wo = intersect->omega;

        Ray light_test_ray(x, wi);
        light_test_ray.pos = light_test_ray.step(eps);
        bool is_intersect_light;
        auto intersect_light =
            objects->ray_intersect(light_test_ray, is_intersect_light);
        if (!is_intersect_light || intersect_light->prim != light.get() ||
            glm::length(intersect_light->pos - x) <
                (1. - eps) * glm::length(x_prime - x)) {
            // test failed, shadow
            continue;
        }

        real cos_theta_i =
            std::max(intersect->normal_dot(wi), static_cast<real>(0));
        real cos_theta_o = std::max(glm::dot(intersect_light->normal, -wi),
                                    static_cast<real>(0));

        vec3 radiance = light->emit_radiance;
        vec3 f = intersect->prim->material->f(wi, wo);
        real G = cos_theta_i * cos_theta_o / glm::length2(x - x_prime);

        direct_radiance += radiance * f * G / light_sinfo->pdf;
    }
    return direct_radiance;
}

HARUNOBU_NAMESPACE_END