#pragma once

#include <harunobu/core/basic.h>
#include <harunobu/integrator/integrator_base.h>

HARUNOBU_NAMESPACE_BEGIN

class RayTracer : public IntegratorBase {
protected:
    int max_depth;

public:
    RayTracer(sptr<Scene> scene_, int max_depth_);

    virtual sptr<Image<real>> integrate();

protected:
    virtual vec3 integrate_ray(const Ray &ray, sptr<SamplerBase> sampler, int depth = 0) = 0;
}; // class RayTracer

HARUNOBU_NAMESPACE_END