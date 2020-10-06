#pragma once

#include <harunobu/core/common.h>

namespace harunobu {

class Ray {
public:
    vec3 pos;
    vec3 dir;

public:
    explicit Ray(const vec3& pos_, const vec3& dir_):
        pos(pos_), dir(dir_) {}
};

} // namespace harunobu