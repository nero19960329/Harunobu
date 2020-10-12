#include <harunobu/core/scene.h>
#include <harunobu/integrator/path_tracer.h>

HARUNOBU_NAMESPACE_BEGIN

PathTracer::PathTracer(sptr<Scene> scene_, int max_depth_)
    : IntegratorBase(scene_), max_depth(max_depth_) {}

cv::Mat PathTracer::integrate() {
    cv::Mat raw_image(film->height, film->width, CV_32FC3, CV_RGB(0., 0., 0.));

#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < raw_image.rows; ++i) {
        if (i % 100 == 0) {
            HARUNOBU_INFO("Rendering row {} ...", i);
        }
        for (int j = 0; j < raw_image.cols; ++j) {
            int sample_count = sampler->sample_count;
            for (int k = 0; k < sample_count; ++k) {
                auto ij = sampler->sample_screen_coor(i, j);
                auto rgb = integrate_ray(scene->camera->make_ray(
                    ij[0], ij[1], film->height, film->width));
                cv::Vec3f bgr(rgb[2], rgb[1], rgb[0]);
                film->add_sample(bgr, ij[0], ij[1]);
            }
        }
    }
    return film->make_image();
}

vec3 PathTracer::integrate_ray(const Ray &ray) {
    bool is_intersect;
    auto intersect = scene->objects->ray_intersect(ray, is_intersect);

    if (!is_intersect) {
        return vec3(0.0, 0.0, 0.0);
    }

    // emission
    vec3 color = intersect->prim->emit_radiance;

    // direct illumination
    // sum over all lights
    for (const auto &light : scene->lights->prims) {
        vec3 x_prime = light->random_sample();
        vec3 x = intersect->pos;
        vec3 wi = glm::normalize(x_prime - x);
        vec3 wo = -ray.dir;

        Ray light_test_ray(x, wi);
        light_test_ray.pos = light_test_ray.step(eps);
        bool is_intersect_light;
        auto intersect_light =
            scene->objects->ray_intersect(light_test_ray, is_intersect_light);
        if (!is_intersect_light || intersect_light->prim != light.get()) {
            // test failed, shadow
            continue;
        }

        real cos_theta_i =
            std::max(intersect->normal_dot(wi), static_cast<real>(0));
        real cos_theta_o = std::max(glm::dot(intersect_light->normal, -wi),
                                    static_cast<real>(0));

        real area = light->area;
        vec3 radiance = light->emit_radiance;
        vec3 f = intersect->prim->material->f(wi, wo);
        real G = cos_theta_i * cos_theta_o / glm::length2(x - x_prime);

        color += radiance * area * f * G;
    }

    return color;
}

HARUNOBU_NAMESPACE_END