#pragma once

#include <harunobu/core/basic.h>
#include <harunobu/core/common.h>

HARUNOBU_NAMESPACE_BEGIN

class Camera {
public:
    // default perspective
    vec3 pos, dir, up;
    real fov;
    int height, width;

public:
    Camera(const vec3 &pos_, const vec3 &dir_, const vec3 &up_, real fov_,
           int height_, int width_);

    Ray make_ray(real i, real j) const;

    void log_current_status() const;
}; // class Camera

HARUNOBU_NAMESPACE_END