#pragma once

#include <harunobu/integrator/ray_tracer.h>

HARUNOBU_NAMESPACE_BEGIN

class PathTracer : public RayTracer {
public:
    int max_depth;

public:
    PathTracer(sptr<Scene> scene_, int max_depth_);

private:
    virtual vec3 integrate_ray(const Ray &ray, int depth = 0) override;
}; // class PathTracer

HARUNOBU_NAMESPACE_END