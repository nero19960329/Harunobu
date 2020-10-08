#pragma once

#include <string>
#include <vector>

#include <harunobu/core/common.h>
#include <harunobu/geometry/geometry_base.h>
#include <harunobu/material/material_base.h>

HARUNOBU_NAMESPACE_BEGIN

class PrimitiveBase {
public:
    sptr<MaterialBase> material;
    std::vector<sptr<GeometryBase>> geos;

public:
    PrimitiveBase(sptr<MaterialBase> material_);

    virtual void log_current_status() const = 0;

    static sptr<PrimitiveBase> factory(std::string name,
                                       sptr<MaterialBase> material,
                                       const mat4 &trans_mat);

    virtual void make_geos(const mat4 &trans_mat) = 0;
}; // class PrimitiveBase

HARUNOBU_NAMESPACE_END