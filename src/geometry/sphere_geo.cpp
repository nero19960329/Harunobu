#include <harunobu/geometry/sphere_geo.h>

HARUNOBU_NAMESPACE_BEGIN

SphereGeo::SphereGeo(const PrimitiveBase *parent_prim_, const vec3 &center_,
                     real radius_)
    : GeometryBase(parent_prim_), center(center_), radius(radius_) {
    area = 4. * pi() * radius * radius;
}

void SphereGeo::do_transform(const mat4 &trans_mat) {
    HARUNOBU_CHECK(false, "SphereGeo does not support do_transform!");
}

sptr<Intersect> SphereGeo::ray_intersect(const Ray &ray, bool &is_intersect) {
    sptr<Intersect> intersect = std::make_shared<Intersect>();
    vec3 sr = ray.pos - center;
    real dot_dir_sr = glm::dot(ray.dir, sr);
    real d = dot_dir_sr * dot_dir_sr - glm::length2(sr) + radius * radius;
    if (d < 0) {
        is_intersect = false;
        return intersect;
    }

    real t1 = -dot_dir_sr - std::sqrt(d);
    real t2 = -dot_dir_sr + std::sqrt(d);
    if (t2 < 0) {
        is_intersect = false;
        return intersect;
    } else if (t1 < 0 && t2 > 0) {
        intersect->ray_step = t2;
    } else if (t1 > 0) {
        intersect->ray_step = t1;
    }
    intersect->pos = ray.step(intersect->ray_step);
    intersect->prim = parent_prim;
    intersect->normal = glm::normalize(intersect->pos - center);
    intersect->omega = -ray.dir;
    is_intersect = true;
    return intersect;
}

sptr<SampleInfo> SphereGeo::random_sample() {
    // x = \sqrt{1 - u^2}cos\theta
    // y = \sqrt{1 - u^2}sin\theta
    // z = u
    // u \in [-1, 1], \theta \in [0, 2\pi)
    real u = rng.random_real(-1, 1);
    real theta = rng.random_real(0, 2 * pi());
    real t = std::sqrt(1 - u * u);
    vec3 unit_sample(t * cos(theta), t * sin(theta), u);
    sptr<SampleInfo> sinfo = std::make_shared<SampleInfo>();
    sinfo->pos = unit_sample * radius + center;
    sinfo->pdf = 1.0 / area;
    return sinfo;
}

void SphereGeo::log_current_status() const {
    HARUNOBU_INFO("SphereGeo");
    HARUNOBU_INFO("center = {}", glm::to_string(center));
    HARUNOBU_INFO("radius = {}", radius);
}

HARUNOBU_NAMESPACE_END