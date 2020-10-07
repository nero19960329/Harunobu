#pragma once

#include <string>
#include <vector>

#include <harunobu/core/common.h>
#include <harunobu/geometry/geometry_base.h>
#include <harunobu/material/material_base.h>

namespace harunobu {

class PrimitiveBase {
public:
    sptr<MaterialBase> material;
    std::vector<sptr<GeometryBase>> geos;

public:
    PrimitiveBase(sptr<MaterialBase> material_, const mat4 &trans_mat);

    // static sptr<PrimitiveBase> factory(std::string name);

private:
    virtual void make_geos(const mat4 &trans_mat) {}
}; // class PrimitiveBase

} // namespace harunobu