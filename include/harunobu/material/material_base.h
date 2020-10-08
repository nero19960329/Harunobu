#pragma once

#include <harunobu/core/common.h>

HARUNOBU_NAMESPACE_BEGIN

class MaterialBase {
public:
    std::string name;
    vec3 rgb;

public:
    MaterialBase() : rgb(0, 0, 0) {}

    virtual void log_current_status() const;
}; // class MaterialBase

HARUNOBU_NAMESPACE_END