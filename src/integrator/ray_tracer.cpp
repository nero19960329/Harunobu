#include <harunobu/core/scene.h>
#include <harunobu/integrator/ray_tracer.h>

#include <indicators/progress_bar.hpp>

HARUNOBU_NAMESPACE_BEGIN

RayTracer::RayTracer(sptr<Scene> scene_, int max_depth_)
    : IntegratorBase(scene_), max_depth(max_depth_) {}

sptr<Image<real>> RayTracer::integrate() {
    using namespace indicators;

    auto raw_image = std::make_shared<Image<real>>(
        std::array<size_t, 3>{3, film->height, film->width});

    ProgressBar bar{
        option::BarWidth{50},
        option::Start{"["},
        option::Fill{"="},
        option::Lead{">"},
        option::Remainder{" "},
        option::End{"]"},
        option::PrefixText{"Rendering ..."},
        option::ForegroundColor{Color::green},
        option::ShowElapsedTime{true},
        option::ShowRemainingTime{true},
        option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}};
    real inv_sum = one * 100 / raw_image->width;
    size_t ray_idx = 0;

#pragma omp parallel for schedule(dynamic)
    for (size_t i = 0; i < raw_image->width; ++i) {
        for (size_t j = 0; j < raw_image->height; ++j) {
            for (int k = 0; k < sampler->sample_count; ++k) {
                vec2 delta = sampler->next_2D() - vec2(.5, .5);
                vec2 ij = vec2(i, j) + delta;
                auto rgb = integrate_ray(scene->camera->make_ray(
                    ij[0], ij[1], film->height, film->width));
#pragma omp critical
                { film->add_sample(rgb, ij[0], ij[1]); }
            }
            sampler->next_pixel();
        }
#pragma omp critical
        { ray_idx++; }
        bar.set_progress(ray_idx * inv_sum);
    }

    return film->make_image();
}

HARUNOBU_NAMESPACE_END