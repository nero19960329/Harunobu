#pragma once

#include <array>

#include <harunobu/geometry/geometry_base.h>

namespace harunobu {

class Tri : public GeometryBase {
public:
    std::array<vec3, 3> vertices;
    std::array<vec3, 3> normals;

public:
    Tri(const PrimitiveBase *parent_prim_);

    virtual vec3 get_normal(const vec3 &p) const;

    virtual void do_transform(const mat4 &trans_mat);
}; // class Tri

} // namespace harunobu