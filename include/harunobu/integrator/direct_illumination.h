#pragma once

#include <harunobu/core/basic.h>
#include <harunobu/integrator/integrator_base.h>

HARUNOBU_NAMESPACE_BEGIN

class DirectIllumination : public IntegratorBase {
public:
    int max_depth;
    int light_sample_num;

public:
    DirectIllumination(sptr<Scene> scene_, int max_depth_,
                       int light_sample_num_);

    virtual cv::Mat integrate() override;

private:
    vec3 integrate_ray(const Ray &ray);
}; // class DirectIllumination

HARUNOBU_NAMESPACE_END