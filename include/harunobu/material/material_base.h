#pragma once

#include <harunobu/core/common.h>
#include <harunobu/core/param_set.h>
#include <harunobu/geometry/geometry_base.h>

HARUNOBU_NAMESPACE_BEGIN

class MaterialBase {
public:
    std::string name;
    vec3 rgb;

    bool is_two_sided;

public:
    MaterialBase() : rgb(0, 0, 0), is_two_sided(false) {}

    virtual vec3 f(sptr<LocalInfo> linfo) const = 0;
    virtual void sample(sptr<LocalInfo> linfo, sptr<SamplerBase> sampler) const;
    virtual real pdf(sptr<LocalInfo> linfo) const;
    virtual void log_current_status() const;

    static sptr<MaterialBase> factory(std::string name, ParamSet &param_set);
}; // class MaterialBase

HARUNOBU_NAMESPACE_END