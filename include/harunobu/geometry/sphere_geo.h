#pragma once

#include <harunobu/geometry/geometry_base.h>

HARUNOBU_NAMESPACE_BEGIN

class SphereGeo : public GeometryBase {
    friend class Intersect;

public:
    vec3 center;
    real radius;

public:
    SphereGeo(const PrimitiveBase *parent_prim_, const vec3 &center_,
              real radius_);

    virtual void do_transform(const mat4 &trans_mat) override;
    virtual sptr<Intersect> ray_intersect(const Ray &ray,
                                          bool &is_intersect) const override;
    virtual sptr<SampleInfo> random_sample() const override;
    virtual sptr<SampleInfo>
    light_sample(sptr<Intersect> intersect) const override;
    virtual real light_sample_pdf(sptr<Intersect> intersect,
                                  sptr<SampleInfo> sinfo) const override;

    virtual void log_current_status() const override;
}; // class SphereGeo

HARUNOBU_NAMESPACE_END