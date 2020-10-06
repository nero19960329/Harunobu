#pragma once

#include <harunobu/core/common.h>

namespace harunobu {

class MaterialBase {
public:
    vec3 rgb;

public:
    MaterialBase() : rgb(0.0, 0.0, 0.0) {}
}; // class MaterialBase

} // namespace harunobu