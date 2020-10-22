#include <harunobu/core/scene.h>
#include <harunobu/integrator/path_tracer.h>
#include <harunobu/integrator/render_utils.h>

HARUNOBU_NAMESPACE_BEGIN

PathTracer::PathTracer(sptr<Scene> scene_, int max_depth_)
    : IntegratorBase(scene_), max_depth(max_depth_) {}

sptr<Image<real>> PathTracer::integrate() {
    auto raw_image = std::make_shared<Image<real>>(
        std::array<size_t, 3>{3, film->height, film->width});

#pragma omp parallel for schedule(dynamic)
    for (size_t i = 0; i < raw_image->width; ++i) {
        if (i % 100 == 0) {
            HARUNOBU_INFO("Rendering col {} ...", i);
        }
        for (size_t j = 0; j < raw_image->height; ++j) {
            int sample_count = sampler->sample_count;
            for (int k = 0; k < sample_count; ++k) {
                auto ij = sampler->sample_screen_coor(i, j);
                auto rgb = integrate_ray(scene->camera->make_ray(
                    ij[0], ij[1], film->height, film->width));
#pragma omp critical
                { film->add_sample(rgb, ij[0], ij[1]); }
            }
        }
    }
    return film->make_image();
}

vec3 PathTracer::integrate_ray(const Ray &ray, int depth) {
    bool is_intersect;
    auto intersect = scene->objects->ray_intersect(ray, is_intersect);

    if (!is_intersect) {
        return vec3(0, 0, 0);
    }

    // emission
    vec3 color = vec3(0, 0, 0);
    if (depth == 0) {
        color += intersect->prim->emit_radiance;
    }

    // direct illumination
    color += RenderUtils::get_direct_radiance(scene->lights->prims, intersect,
                                              scene->objects, 1, 1);

    vec3 x = intersect->pos;
    vec3 wo = intersect->omega;
    sptr<LocalInfo> linfo = std::make_shared<LocalInfo>(
        vec3(), wo, intersect->normal, intersect->prim->material->is_two_sided);
    intersect->prim->material->sample(linfo);
    real pdf = intersect->prim->material->pdf(linfo);
    vec3 wi = linfo->to_world(linfo->wi);
    real cos_theta_i =
        std::max(linfo->normal_dot(linfo->wi), static_cast<real>(0));
    vec3 t = intersect->prim->material->f(linfo) * cos_theta_i / pdf;
    real contrib_ratio = std::max(t[0], std::max(t[1], t[2]));
    real survival_prob = std::max(std::min(contrib_ratio, .95), .05);
    if (depth < 3) {
        survival_prob = 1.;
    }

    if (rng.random_real() < survival_prob && depth < max_depth) {
        Ray reflected_ray(x, wi);
        reflected_ray.pos = reflected_ray.step(eps);

        vec3 reflected_radiance = t * integrate_ray(reflected_ray, depth + 1);
        color += reflected_radiance / survival_prob;
    }

    return color;
}

HARUNOBU_NAMESPACE_END