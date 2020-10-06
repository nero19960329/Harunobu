#pragma once

#include <harunobu/core/common.h>

namespace harunobu {

inline vec3 homo2carte(const vec4& v) {
    HARUNOBU_ASSERT(v[3] != 0.0, "Homogeneous element equals to 0!");
    return vec3(v / v[3]);
}

} // namespace harunobu