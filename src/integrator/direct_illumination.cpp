#include <harunobu/core/scene.h>
#include <harunobu/integrator/direct_illumination.h>
#include <harunobu/integrator/render_utils.h>

HARUNOBU_NAMESPACE_BEGIN

DirectIllumination::DirectIllumination(sptr<Scene> scene_, int max_depth_,
                                       int light_sample_num_)
    : IntegratorBase(scene_), max_depth(max_depth_),
      light_sample_num(light_sample_num_) {}

sptr<Image<real>> DirectIllumination::integrate() {
    auto raw_image = std::make_shared<Image<real>>(std::array<size_t, 3>{ 3, film->height, film->width });

#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < raw_image->width; ++i) {
        if (i % 100 == 0) {
            HARUNOBU_INFO("Rendering row {} ...", i);
        }
        for (int j = 0; j < raw_image->height; ++j) {
            int sample_count = sampler->sample_count;
            for (int k = 0; k < sample_count; ++k) {
                auto ij = sampler->sample_screen_coor(i, j);
                auto rgb = integrate_ray(scene->camera->make_ray(
                    ij[0], ij[1], film->height, film->width));
#pragma omp critical
{
                film->add_sample(rgb, ij[0], ij[1]);
}
            }
        }
    }
    return film->make_image();
}

vec3 DirectIllumination::integrate_ray(const Ray &ray) {
    bool is_intersect;
    auto intersect = scene->objects->ray_intersect(ray, is_intersect);

    if (!is_intersect) {
        return vec3(0.0, 0.0, 0.0);
    }

    // emission
    vec3 color = intersect->prim->emit_radiance;

    // direct illumination
    color += RenderUtils::get_direct_radiance(scene->lights->prims, intersect,
                                              scene->objects);

    return color;
}

HARUNOBU_NAMESPACE_END