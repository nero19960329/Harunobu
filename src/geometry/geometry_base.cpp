#include <harunobu/geometry/geometry_base.h>

HARUNOBU_NAMESPACE_BEGIN

LocalInfo::LocalInfo(const vec3 &wi_world, const vec3 &wo_world,
                     const vec3 &normal_world, bool is_two_sided_)
    : is_two_sided(is_two_sided_) {
    vec3 w = normal_world;
    vec3 v = glm::normalize(glm::cross(w, wo_world));
    vec3 u = glm::normalize(glm::cross(v, w));
    T = mat3(u, v, w);
    T_inv = glm::inverse(T);
    wi = to_local(wi_world);
    wo = to_local(wo_world);
}

vec3 LocalInfo::to_world(const vec3 &w) const { return w * T_inv; }

vec3 LocalInfo::to_local(const vec3 &w) const { return w * T; }

real LocalInfo::normal_dot(const vec3 &w) const {
    if (is_two_sided) {
        return std::abs(w.z);
    } else {
        return w.z;
    }
}

sptr<SampleInfo> GeometryBase::light_sample(sptr<Intersect> intersect) const {
    auto sinfo = random_sample();
    sinfo->pdf = light_sample_pdf(intersect, sinfo);
    return sinfo;
}

real GeometryBase::light_sample_pdf(sptr<Intersect> intersect,
                                    sptr<SampleInfo> sinfo) const {
    vec3 x = intersect->pos;
    vec3 x_prime = sinfo->pos;
    real abs_cos_theta_o =
        std::abs(glm::dot(sinfo->normal, glm::normalize(x - x_prime)));
    if (abs_cos_theta_o > 0.0) {
        return random_sample_pdf(sinfo) * glm::length2(x - x_prime) /
               abs_cos_theta_o;
    } else {
        return 0.0;
    }
}

HARUNOBU_NAMESPACE_END