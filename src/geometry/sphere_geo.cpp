#include <harunobu/core/utils.h>
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

sptr<Intersect> SphereGeo::ray_intersect(const Ray &ray,
                                         bool &is_intersect) const {
    sptr<Intersect> intersect = std::make_shared<Intersect>();
    vec3 sr = ray.pos - center;
    real dot_dir_sr = glm::dot(ray.dir, sr);
    real d = dot_dir_sr * dot_dir_sr - glm::length2(sr) + radius * radius;
    if (d < 0) {
        is_intersect = false;
        return intersect;
    }

    real t1 = -dot_dir_sr - safe_sqrt(d);
    real t2 = -dot_dir_sr + safe_sqrt(d);
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
    intersect->geo = this;
    intersect->normal = glm::normalize(intersect->pos - center);
    intersect->omega = -ray.dir;
    is_intersect = true;
    return intersect;
}

sptr<SampleInfo> SphereGeo::random_sample() const {
    // x = \sqrt{1 - u^2}cos\theta
    // y = \sqrt{1 - u^2}sin\theta
    // z = u
    // u \in [-1, 1], \theta \in [0, 2\pi)
    real u = rng.random_real(-1, 1);
    real theta = rng.random_real(0, 2 * pi());
    real t = safe_sqrt(1 - u * u);
    vec3 unit_sample(t * cos(theta), t * sin(theta), u);
    sptr<SampleInfo> sinfo = std::make_shared<SampleInfo>();
    sinfo->pos = unit_sample * radius + center;
    sinfo->pdf = random_sample_pdf();
    sinfo->normal = glm::normalize(sinfo->pos - center);
    sinfo->prim = parent_prim;
    return sinfo;
}

sptr<SampleInfo> SphereGeo::light_sample(sptr<Intersect> intersect) const {
    vec3 x = intersect->pos;
    vec3 w = glm::normalize(center - x);
    vec3 v = glm::normalize(glm::cross(w, intersect->normal));
    vec3 u = glm::normalize(glm::cross(v, w));
    if (glm::any(glm::isnan(v)) || glm::any(glm::isnan(u))) {
        return GeometryBase::light_sample(intersect);
    }

    real t = 1 - safe_sqrt(1 - radius * radius / glm::length2(center - x));
    real theta = std::acos(1 - t * rng.random_real());
    real phi = rng.random_real(0, 2 * pi());
    vec3 a = vec3(cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta)) *
             mat3(u[0], v[0], w[0], u[1], v[1], w[1], u[2], v[2], w[2]);
    Ray ray(x, glm::normalize(a));
    ray.pos = ray.step(eps);
    bool is_intersect;
    auto light_intersect = ray_intersect(ray, is_intersect);
    if (!is_intersect) {
        return GeometryBase::light_sample(intersect);
    }

    vec3 x_prime = light_intersect->pos;
    sptr<SampleInfo> sinfo = std::make_shared<SampleInfo>();
    sinfo->pos = x_prime;
    sinfo->pdf = light_sample_pdf(x, x_prime, light_intersect->normal);
    sinfo->normal = light_intersect->normal;
    sinfo->prim = light_intersect->prim;
    if (sinfo->pdf <= 0.0) {
        return GeometryBase::light_sample(intersect);
    }
    return sinfo;
}

real SphereGeo::light_sample_pdf(const vec3 &x, const vec3 &x_light,
                                 const vec3 &n_light) const {
    real t = 1 - safe_sqrt(1 - radius * radius / glm::length2(center - x));
    return 1.0 / (2 * pi() * t);
}

void SphereGeo::log_current_status() const {
    HARUNOBU_INFO("SphereGeo");
    HARUNOBU_INFO("center = {}", glm::to_string(center));
    HARUNOBU_INFO("radius = {}", radius);
}

HARUNOBU_NAMESPACE_END