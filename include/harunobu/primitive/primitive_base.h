#pragma once

#include <vector>

#include <harunobu/core/common.h>
#include <harunobu/geometry/geometry_base.h>
#include <harunobu/material/material_base.h>

namespace harunobu {

class PrimitiveBase {
public:
    sptr<MaterialBase> material;
    std::vector<GeometryBase> geos;
}; // class PrimitiveBase

} // namespace harunobu