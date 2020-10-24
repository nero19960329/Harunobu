#include <harunobu/core/scene.h>
#include <harunobu/integrator/ray_tracer.h>

#include <indicators/progress_bar.hpp>
#include <omp.h>

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
    size_t pixel_num = raw_image->width * raw_image->height;
    real inv_sum = one * 100 / pixel_num;
    size_t ray_idx = 0;

    size_t thread_num = omp_get_max_threads();
    std::vector<sptr<SamplerBase>> samplers(thread_num);
    for (size_t i = 0; i < thread_num; ++i) {
        samplers[i] = sampler->copy();
    }

#pragma omp parallel for schedule(dynamic)
    for (size_t p = 0; p < pixel_num; ++p) {
        int thread_id = omp_get_thread_num();
        sptr<SamplerBase> sampler_thread = samplers[thread_id];

        size_t i = p / raw_image->height, j = p % raw_image->height;
        for (int k = 0; k < sampler_thread->sample_count; ++k) {
            vec2 delta = sampler_thread->next_2D() - vec2(.5, .5);
            vec2 ij = vec2(i, j) + delta;
            auto rgb = integrate_ray(scene->camera->make_ray(
                ij[0], ij[1], raw_image->height, raw_image->width), sampler_thread);
#pragma omp critical
            { film->add_sample(rgb, ij[0], ij[1]); }
        }
        sampler_thread->next_pixel();
#pragma omp critical
        { ray_idx++; }
        bar.set_progress(ray_idx * inv_sum);
    }

    return film->make_image();
}

HARUNOBU_NAMESPACE_END