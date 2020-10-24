#include <harunobu/core/scene.h>
#include <harunobu/integrator/direct_illumination.h>
#include <harunobu/integrator/render_utils.h>

HARUNOBU_NAMESPACE_BEGIN

DirectIllumination::DirectIllumination(sptr<Scene> scene_, int max_depth_,
                                       int light_sample_num_,
                                       int bsdf_sample_num_)
    : RayTracer(scene_, max_depth_), light_sample_num(light_sample_num_),
      bsdf_sample_num(bsdf_sample_num_) {}

vec3 DirectIllumination::integrate_ray(const Ray &ray, sptr<SamplerBase> sampler, int depth) {
    bool is_intersect;
    auto intersect = scene->objects->ray_intersect(ray, is_intersect);

    if (!is_intersect) {
        return vec3(0, 0, 0);
    }

    // emission
    vec3 color = intersect->prim->emit_radiance;

    // direct illumination
    color += RenderUtils::get_direct_radiance(scene->lights->prims, intersect,
                                              scene->objects, sampler, light_sample_num,
                                              bsdf_sample_num);

    return color;
}

HARUNOBU_NAMESPACE_END