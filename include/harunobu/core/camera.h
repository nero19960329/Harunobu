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
    Camera();

    void log_current_status() const;
}; // class Camera

} // namespace harunobu