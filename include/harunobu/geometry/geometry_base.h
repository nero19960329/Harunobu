#pragma once

#include <functional>

#include <harunobu/core/basic.h>
#include <harunobu/core/common.h>

HARUNOBU_NAMESPACE_BEGIN

class PrimitiveBase;

class Intersect {
public:
    real ray_step;
    vec3 pos, normal;
    vec3 omega;
    const PrimitiveBase *prim;

    Intersect() {}
}; // class Intersect

struct SampleInfo {
    vec3 pos, normal;
    real pdf;
    const PrimitiveBase *prim;
}; // struct SampleInfo

class GeometryBase {
    friend class PrimitiveBase;

public:
    const PrimitiveBase *parent_prim;

protected:
    real area;

public:
    GeometryBase(const PrimitiveBase *parent_prim_)
        : parent_prim(parent_prim_) {}

    virtual void do_transform(const mat4 &trans_mat) = 0;
    virtual sptr<Intersect> ray_intersect(const Ray &ray,
                                          bool &is_intersect) const = 0;
    virtual sptr<SampleInfo> random_sample() const = 0;
    virtual sptr<SampleInfo> light_sample(sptr<Intersect> intersect) const {
        return random_sample();
    }

    virtual void log_current_status() const = 0;
}; // class GeometryBase

HARUNOBU_NAMESPACE_END