#include <harunobu/core/utils.h>
#include <harunobu/geometry/tri.h>

namespace harunobu {

Tri::Tri(const PrimitiveBase* parent_prim_)
    : GeometryBase(parent_prim_) {
    vertices[0] = vec3(0.0, 0.0, 0.0);
    vertices[1] = vec3(1.0, 0.0, 0.0);
    vertices[2] = vec3(0.0, 1.0, 0.0);
    normals[0] = normals[1] = normals[2] = vec3(0.0, 0.0, 1.0);
}

vec3 Tri::get_normal(const vec3 &p) const {
    HARUNOBU_CHECK(false, "Not implemented.")
    return vec3();
}

void Tri::do_transform(const mat4 &trans_mat) {
    for (auto& v : vertices) {
        v = homo2carte(vec4(v, 1.0) * trans_mat);
    }
    for (auto& n : normals) {
        n = vec4(n, 0.0) * trans_mat;
    }
}

} // namespace harunobu