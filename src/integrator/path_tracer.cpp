#include <harunobu/core/scene.h>
#include <harunobu/integrator/path_tracer.h>

HARUNOBU_NAMESPACE_BEGIN

PathTracer::PathTracer(sptr<Scene> scene_, int max_depth_)
    : IntegratorBase(scene_), max_depth(max_depth_) {}

cv::Mat PathTracer::integrate() {
    sptr<Camera> camera = scene->camera;
    cv::Mat raw_image(camera->height, camera->width, CV_32FC3,
                      CV_RGB(0., 0., 0.));
    for (int i = 0; i < raw_image.rows; ++i) {
        for (int j = 0; j < raw_image.cols; ++j) {
            auto rgb = integrate_ray(camera->make_ray(i, j));
            // bgr(cv) <- rgb
            raw_image.at<cv::Vec3f>(j, i) = cv::Vec3f(rgb[2], rgb[1], rgb[0]);
        }
    }
    return raw_image;
}

vec3 PathTracer::integrate_ray(const Ray &ray) {
    bool is_intersect;
    auto intersect = scene->objects->ray_intersect(ray, is_intersect);

    if (is_intersect) {
        return intersect->prim->material->rgb;
    } else {
        return vec3(0.0, 0.0, 0.0);
    }
}

HARUNOBU_NAMESPACE_END