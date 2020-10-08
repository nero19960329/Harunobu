#include <harunobu/core/scene.h>
#include <harunobu/integrator/path_tracer.h>

HARUNOBU_NAMESPACE_BEGIN

PathTracer::PathTracer(sptr<Scene> scene_, int max_depth_)
    : IntegratorBase(scene_), max_depth(max_depth_) {}

cv::Mat PathTracer::integrate() {
    return cv::Mat(scene->camera->height, scene->camera->width, CV_32FC3,
                   CV_RGB(1.2, .2, .2));
}

HARUNOBU_NAMESPACE_END