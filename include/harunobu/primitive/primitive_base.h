#pragma once

#include <string>
#include <vector>

#include <harunobu/core/common.h>
#include <harunobu/core/param_set.h>
#include <harunobu/geometry/geometry_base.h>
#include <harunobu/material/material_base.h>

HARUNOBU_NAMESPACE_BEGIN

class PrimitiveBase {
public:
    sptr<MaterialBase> material;
    vec3 emit_radiance;
    std::vector<sptr<GeometryBase>> geos;

    real area;

public:
    PrimitiveBase(sptr<MaterialBase> material_);

    virtual void log_current_status() const = 0;

    static sptr<PrimitiveBase>
    factory(std::string name, sptr<MaterialBase> material, ParamSet &param_set);

    virtual void init(ParamSet &param_set) = 0;
    virtual vec3 random_sample() = 0;

protected:
    void make_area();
}; // class PrimitiveBase

HARUNOBU_NAMESPACE_END