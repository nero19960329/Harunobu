#pragma once

#include <harunobu/core/basic.h>
#include <harunobu/core/common.h>
#include <harunobu/geometry/geometry_base.h>
#include <harunobu/objects/objects_base.h>
#include <harunobu/primitive/primitive_base.h>

HARUNOBU_NAMESPACE_BEGIN

class RenderUtils {
public:
    static vec3 get_direct_radiance(std::vector<sptr<PrimitiveBase>> lights,
                                    sptr<Intersect> intersect,
                                    sptr<ObjectsBase> objects);
}; // class RenderUtils

HARUNOBU_NAMESPACE_END