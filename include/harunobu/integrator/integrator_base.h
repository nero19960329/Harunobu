#pragma once

#include <harunobu/core/common.h>

#include <opencv2/opencv.hpp>

HARUNOBU_NAMESPACE_BEGIN

class Scene;

class IntegratorBase {
public:
    sptr<Scene> scene;
    // sptr<SamplerBase> sampler;

public:
    IntegratorBase(sptr<Scene> scene_) : scene(scene_) {}

    virtual cv::Mat integrate() = 0;
}; // class IntegratorBase

HARUNOBU_NAMESPACE_END