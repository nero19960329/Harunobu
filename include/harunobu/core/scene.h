#pragma once

#include <harunobu/core/camera.h>
#include <harunobu/core/common.h>
#include <harunobu/integrator/integrator_base.h>
#include <harunobu/objects/objects_base.h>

#include <opencv2/opencv.hpp>

namespace harunobu {

class Scene {
public:
    sptr<Camera> camera;
    sptr<ObjectsBase> objects;
    sptr<IntegratorBase> integrator;

public:
    Scene() {}

    cv::Mat render();
}; // class Scene

} // namespace harunobu