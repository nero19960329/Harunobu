#include <harunobu/core/scene.h>
#include <harunobu/integrator/path_tracer.h>
#include <harunobu/integrator/render_utils.h>

HARUNOBU_NAMESPACE_BEGIN

PathTracer::PathTracer(sptr<Scene> scene_, int max_depth_)
    : RayTracer(scene_, max_depth_) {}

vec3 PathTracer::integrate_ray(const Ray &ray, sptr<SamplerBase> sampler, int depth) {
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
                                              scene->objects, sampler, 1, 1);

    vec3 x = intersect->pos;
    vec3 wo = intersect->omega;
    sptr<LocalInfo> linfo = std::make_shared<LocalInfo>(
        vec3(), wo, intersect->normal, intersect->prim->material->is_two_sided);
    intersect->prim->material->sample(linfo, sampler);
    real pdf = intersect->prim->material->pdf(linfo);
    vec3 wi = linfo->to_world(linfo->wi);
    real cos_theta_i = std::max(linfo->normal_dot(linfo->wi), zero);
    vec3 t = intersect->prim->material->f(linfo) * cos_theta_i / pdf;
    real contrib_ratio = std::max(t[0], std::max(t[1], t[2]));
    real survival_prob = std::max(std::min(contrib_ratio, .95), .05);
    if (depth < 3) {
        survival_prob = 1.;
    }

    if (sampler->next_1D() < survival_prob && depth < max_depth) {
        Ray reflected_ray(x, wi);
        reflected_ray.pos = reflected_ray.step(eps);

        vec3 reflected_radiance = t * integrate_ray(reflected_ray, sampler, depth + 1);
        color += reflected_radiance / survival_prob;
    }

    return color;
}

HARUNOBU_NAMESPACE_END