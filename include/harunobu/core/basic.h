#pragma once

#include <harunobu/core/common.h>

HARUNOBU_NAMESPACE_BEGIN

class Ray {
public:
    vec3 pos;
    vec3 dir;

public:
    explicit Ray(const vec3 &pos_, const vec3 &dir_) : pos(pos_), dir(dir_) {}

    inline vec3 step(real t) const { return pos + dir * t; }
};

HARUNOBU_NAMESPACE_END