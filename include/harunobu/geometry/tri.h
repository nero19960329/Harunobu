#pragma once

#include <array>

#include <harunobu/geometry/geometry_base.h>

HARUNOBU_NAMESPACE_BEGIN

class Tri : public GeometryBase {
    friend class Intersect;

public:
    std::array<vec3, 3> vertices;
    std::array<vec3, 3> normals;

protected:
    vec3 normal_geo;

public:
    Tri(const PrimitiveBase *parent_prim_, const std::array<vec3, 3> &vertices_,
        const std::array<vec3, 3> &normals_);

    virtual void do_transform(const mat4 &trans_mat) override;
    virtual sptr<Intersect> ray_intersect(const Ray &ray,
                                          bool &is_intersect) const override;
    virtual sptr<SampleInfo> random_sample() const override;

    virtual void log_current_status() const override;

private:
    vec3 get_barycentric_coordinate(const vec3 &pos) const;
}; // class Tri

HARUNOBU_NAMESPACE_END