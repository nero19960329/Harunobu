#pragma once

#include <harunobu/core/basic.h>
#include <harunobu/core/common.h>

HARUNOBU_NAMESPACE_BEGIN

class PrimitiveBase;

struct Intersect {
    real ray_step;
    vec3 pos, normal;
    const PrimitiveBase *prim;

}; // struct Intersect

class GeometryBase {
public:
    const PrimitiveBase *parent_prim;

public:
    GeometryBase(const PrimitiveBase *parent_prim_)
        : parent_prim(parent_prim_) {}

    virtual void do_transform(const mat4 &trans_mat) = 0;
    virtual sptr<Intersect> ray_intersect(const Ray &ray,
                                          bool &is_intersect) = 0;

    virtual void log_current_status() const = 0;
}; // class GeometryBase

HARUNOBU_NAMESPACE_END