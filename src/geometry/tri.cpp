#include <harunobu/core/utils.h>
#include <harunobu/geometry/tri.h>

namespace harunobu {

Tri::Tri(const PrimitiveBase *parent_prim_,
         const std::array<vec3, 3> &vertices_,
         const std::array<vec3, 3> &normals_)
    : GeometryBase(parent_prim_), vertices(vertices_), normals(normals_) {}

vec3 Tri::get_normal(const vec3 &p) const {
    HARUNOBU_CHECK(false, "Not implemented.")
    return vec3();
}

void Tri::do_transform(const mat4 &trans_mat) {
    for (auto &v : vertices) {
        v = homo2carte(vec4(v, 1.0) * trans_mat);
    }
    for (auto &n : normals) {
        n = vec4(n, 0.0) * trans_mat;
        n = glm::normalize(n);
    }
}

void Tri::log_current_status() const {
    HARUNOBU_INFO("Tri");
    HARUNOBU_INFO("vertices = {}, {}, {}", glm::to_string(vertices[0]),
                  glm::to_string(vertices[1]), glm::to_string(vertices[2]));
    HARUNOBU_INFO("normals = {}, {}, {}", glm::to_string(normals[0]),
                  glm::to_string(normals[1]), glm::to_string(normals[2]));
}

} // namespace harunobu