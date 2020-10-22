#pragma once

#include <functional>

#include <harunobu/core/basic.h>
#include <harunobu/core/common.h>

HARUNOBU_NAMESPACE_BEGIN

class GeometryBase;
class PrimitiveBase;

class Intersect {
public:
    real ray_step;
    vec3 pos, normal;
    vec3 omega;
    const PrimitiveBase *prim;
    const GeometryBase *geo;

    Intersect() {}
}; // class Intersect

struct SampleInfo {
    vec3 pos, normal;
    real pdf;
    const PrimitiveBase *prim;
}; // struct SampleInfo

class LocalInfo {
public:
    vec3 wi, wo;
    bool is_two_sided;

private:
    mat3 T, T_inv;

public:
    LocalInfo(const vec3 &wi_world, const vec3 &wo_world,
              const vec3 &normal_world, bool is_two_sided_);

    vec3 to_world(const vec3 &w) const;
    vec3 to_local(const vec3 &w) const;

    real normal_dot(const vec3 &w) const;
    bool same_hemisphere(const vec3 &u, const vec3 &v) const;
}; // class LocalInfo

class GeometryBase {
    friend class PrimitiveBase;

public:
    const PrimitiveBase *parent_prim;
    size_t idx;

protected:
    real area;

public:
    GeometryBase(const PrimitiveBase *parent_prim_)
        : parent_prim(parent_prim_) {}

    virtual void do_transform(const mat4 &trans_mat) = 0;
    virtual sptr<Intersect> ray_intersect(const Ray &ray,
                                          bool &is_intersect) const = 0;
    virtual sptr<SampleInfo> random_sample() const = 0;
    virtual real random_sample_pdf() const { return 1.0 / area; }
    virtual sptr<SampleInfo> light_sample(sptr<Intersect> intersect) const;
    virtual real light_sample_pdf(const vec3 &x, const vec3 &x_light,
                                  const vec3 &n_light) const;

    virtual void log_current_status() const = 0;
}; // class GeometryBase

HARUNOBU_NAMESPACE_END