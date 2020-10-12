#pragma once

#include <harunobu/core/common.h>

HARUNOBU_NAMESPACE_BEGIN

class MaterialBase {
public:
    std::string name;
    vec3 rgb;

    bool is_two_sided;

public:
    MaterialBase() : rgb(0, 0, 0), is_two_sided(false) {}

    virtual vec3 f(const vec3 &wi, const vec3 &wo) const = 0;
    virtual void log_current_status() const;
}; // class MaterialBase

HARUNOBU_NAMESPACE_END