#pragma once

#include <harunobu/core/camera.h>
#include <harunobu/core/common.h>
#include <harunobu/core/image_pipeline.h>
#include <harunobu/integrator/integrator_base.h>
#include <harunobu/objects/objects_base.h>

#include <opencv2/opencv.hpp>

HARUNOBU_NAMESPACE_BEGIN

class Scene {
public:
    sptr<Camera> camera;
    sptr<ObjectsBase> objects;
    sptr<IntegratorBase> integrator;
    sptr<ImagePipeline> image_pipeline;

public:
    Scene() {}

    void render();
}; // class Scene

HARUNOBU_NAMESPACE_END