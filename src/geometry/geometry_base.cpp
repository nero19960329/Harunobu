#include <harunobu/geometry/geometry_base.h>
#include <harunobu/primitive/primitive_base.h>

HARUNOBU_NAMESPACE_BEGIN

real Intersect::normal_dot(const vec3 &v) const {
    if (prim->material->is_two_sided) {
        return std::abs(glm::dot(normal, v));
    } else {
        return glm::dot(normal, v);
    }
}

HARUNOBU_NAMESPACE_END