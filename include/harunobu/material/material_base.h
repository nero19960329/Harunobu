#pragma once

#include <harunobu/core/common.h>

namespace harunobu {

class MaterialBase {
public:
    std::string name;
    vec3 rgb;

public:
    MaterialBase() : rgb(0.0, 0.0, 0.0) {}

    virtual void log_current_status() const;
}; // class MaterialBase

} // namespace harunobu