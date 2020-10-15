#include <harunobu/core/utils.h>
#include <harunobu/geometry/tri.h>
#include <harunobu/primitive/primitive_base.h>

HARUNOBU_NAMESPACE_BEGIN

Tri::Tri(const PrimitiveBase *parent_prim_,
         const std::array<vec3, 3> &vertices_,
         const std::array<vec3, 3> &normals_)
    : GeometryBase(parent_prim_), vertices(vertices_), normals(normals_) {
    auto cross_product =
        glm::cross(vertices[1] - vertices[0], vertices[2] - vertices[1]);
    normal_geo = glm::normalize(cross_product);
    area = glm::length(cross_product) * 0.5;
}

void Tri::do_transform(const mat4 &trans_mat) {
    for (auto &v : vertices) {
        v = homo2carte(vec4(v, 1) * trans_mat);
    }
    for (auto &n : normals) {
        n = vec4(n, 0) * trans_mat;
        n = glm::normalize(n);
    }
    normal_geo = glm::normalize(vec4(normal_geo, 0) * trans_mat);
    area = glm::length(glm::cross(vertices[1] - vertices[0],
                                  vertices[2] - vertices[1])) *
           0.5;
}

sptr<Intersect> Tri::ray_intersect(const Ray &ray, bool &is_intersect) {
    sptr<Intersect> intersect = std::make_shared<Intersect>();
    real t = glm::dot(vertices[0] - ray.pos, normal_geo) /
             glm::dot(ray.dir, normal_geo);
    auto inter_pos = ray.step(t);
    auto bary = get_barycentric_coordinate(inter_pos);
    if (0.0 <= bary[0] && bary[0] <= 1.0 && 0.0 <= bary[1] && bary[1] <= 1.0 &&
        0.0 <= bary[2] && bary[2] <= 1.0 && t >= 0.0) {
        intersect->ray_step = t;
        intersect->pos = inter_pos;
        intersect->prim = parent_prim;
        intersect->normal =
            normals[0] * bary[0] + normals[1] * bary[1] + normals[2] * bary[2];
        intersect->omega = -ray.dir;
        is_intersect = true;
    } else {
        is_intersect = false;
    }
    return intersect;
}

vec3 Tri::get_barycentric_coordinate(const vec3 &pos) const {
    // P = A + u * (C - A) + v * (B - A)
    // P = aA + bB + cC
    vec3 v0 = vertices[2] - vertices[0];
    vec3 v1 = vertices[1] - vertices[0];
    vec3 v2 = pos - vertices[0];

    real dot00 = glm::dot(v0, v0);
    real dot01 = glm::dot(v0, v1);
    real dot02 = glm::dot(v0, v2);
    real dot11 = glm::dot(v1, v1);
    real dot12 = glm::dot(v1, v2);

    real inv_denom = 1.0 / (dot00 * dot11 - dot01 * dot01);
    real u = (dot11 * dot02 - dot01 * dot12) * inv_denom;
    real v = (dot00 * dot12 - dot01 * dot02) * inv_denom;

    return vec3(1.0 - u - v, v, u);
}

sptr<SampleInfo> Tri::random_sample() const {
    // P = (1 - sqrt(r1)) * A + (sqrt(r1) * (1 - r2)) * B + (sqrt(r1) * r2) * C
    real sqrt_u = std::sqrt(rng.random_real());
    real v = rng.random_real();
    real a = 1. - sqrt_u, b = sqrt_u * (1. - v), c = sqrt_u * v;
    sptr<SampleInfo> sinfo = std::make_shared<SampleInfo>();
    sinfo->pos = vec3(a * vertices[0] + b * vertices[1] + c * vertices[2]);
    sinfo->pdf = 1.0 / area;
    auto bary = get_barycentric_coordinate(sinfo->pos);
    sinfo->normal =
        normals[0] * bary[0] + normals[1] * bary[1] + normals[2] * bary[2];
    sinfo->prim = parent_prim;
    return sinfo;
}

void Tri::log_current_status() const {
    HARUNOBU_INFO("Tri");
    HARUNOBU_INFO("vertices = {}, {}, {}", glm::to_string(vertices[0]),
                  glm::to_string(vertices[1]), glm::to_string(vertices[2]));
    HARUNOBU_INFO("normals = {}, {}, {}", glm::to_string(normals[0]),
                  glm::to_string(normals[1]), glm::to_string(normals[2]));
}

HARUNOBU_NAMESPACE_END