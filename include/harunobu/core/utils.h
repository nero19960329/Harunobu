#pragma once

#include <harunobu/core/common.h>

HARUNOBU_NAMESPACE_BEGIN

inline vec3 homo2carte(const vec4 &v) {
    HARUNOBU_ASSERT(v[3] != 0.0, "Homogeneous element equals to 0!");
    return vec3(v / v[3]);
}

HARUNOBU_NAMESPACE_END