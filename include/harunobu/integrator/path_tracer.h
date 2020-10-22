#pragma once

#include <harunobu/core/basic.h>
#include <harunobu/integrator/integrator_base.h>

HARUNOBU_NAMESPACE_BEGIN

class PathTracer : public IntegratorBase {
public:
    int max_depth;

public:
    PathTracer(sptr<Scene> scene_, int max_depth_);

    virtual sptr<Image<real>> integrate() override;

private:
    vec3 integrate_ray(const Ray &ray, int depth = 0);
}; // class PathTracer

HARUNOBU_NAMESPACE_END