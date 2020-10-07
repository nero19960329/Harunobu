#pragma once

#include <harunobu/core/common.h>

namespace harunobu {

class Camera {
public:
    // default perspective
    vec3 pos, dir, up;
    real fov;
    int height, width;

public:
    Camera(const vec3 &pos_, const vec3 &dir_, const vec3 &up_, real fov_,
           int height_, int width_);

    void log_current_status() const;
}; // class Camera

} // namespace harunobu