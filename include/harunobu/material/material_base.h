#pragma once

#include <harunobu/core/common.h>

namespace harunobu {

class MaterialBase {
public:
    std::string name;
    vec3 rgb;

public:
    MaterialBase() : rgb(0.0, 0.0, 0.0) {}
}; // class MaterialBase

} // namespace harunobu