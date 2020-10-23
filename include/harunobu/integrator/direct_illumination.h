#pragma once

#include <harunobu/integrator/ray_tracer.h>

HARUNOBU_NAMESPACE_BEGIN

class DirectIllumination : public RayTracer {
public:
    int light_sample_num, bsdf_sample_num;

public:
    DirectIllumination(sptr<Scene> scene_, int max_depth_,
                       int light_sample_num_, int bsdf_sample_num_);

private:
    virtual vec3 integrate_ray(const Ray &ray, int depth = 0) override;
}; // class DirectIllumination

HARUNOBU_NAMESPACE_END