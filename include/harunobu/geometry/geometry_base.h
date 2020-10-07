#pragma once

#include <harunobu/core/common.h>

namespace harunobu {

class PrimitiveBase;

class GeometryBase {
public:
    const PrimitiveBase* parent_prim;

public:
    GeometryBase(const PrimitiveBase* parent_prim_)
        : parent_prim(parent_prim_) {}

    // maybe input argument's type should be sth like IntersectInfo?
    virtual vec3 get_normal(const vec3 &p) const = 0;

    virtual void do_transform(const mat4 &trans_mat) = 0;
}; // class GeomertyBase

} // namespace harunobu