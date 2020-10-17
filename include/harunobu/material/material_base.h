#pragma once

#include <harunobu/core/common.h>
#include <harunobu/core/param_set.h>

HARUNOBU_NAMESPACE_BEGIN

class MaterialBase {
public:
    std::string name;
    vec3 rgb;

    bool is_two_sided;

public:
    MaterialBase() : rgb(0, 0, 0), is_two_sided(false) {}

    virtual vec3 f(const vec3 &wi, const vec3 &wo,
                   const vec3 &normal) const = 0;
    real normal_dot(const vec3 &normal, const vec3 &w) const;
    virtual void log_current_status() const;

    static sptr<MaterialBase> factory(std::string name, ParamSet &param_set);
}; // class MaterialBase

HARUNOBU_NAMESPACE_END