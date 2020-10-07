#pragma once

#include <harunobu/core/common.h>

#include <opencv2/opencv.hpp>

namespace harunobu {

class Scene;

class IntegratorBase {
public:
    sptr<Scene> scene;
    // sptr<SamplerBase> sampler;

public:
    IntegratorBase(sptr<Scene> scene_) : scene(scene_) {}

    virtual cv::Mat integrate() = 0;
}; // class IntegratorBase

} // namespace harunobu