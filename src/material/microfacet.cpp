#include <harunobu/material/microfacet.h>

HARUNOBU_NAMESPACE_BEGIN

vec3 Microfacet::f(const vec3 &wi, const vec3 &wo, const vec3 &normal) const {
    vec3 wh = glm::normalize(wi + wo);
    real cos_theta_i = normal_dot(normal, wi);
    real cos_theta_o = normal_dot(normal, wo);
    if (cos_theta_i <= 0 || cos_theta_o <= 0) {
        return vec3(0, 0, 0);
    }

    auto d_value = D(wh, normal);
    auto g_value = G(wo, wi, normal);
    auto fr_value = fresnel(wo, wh);
    auto sr = specular_reflectance;

    return D(wh, normal) * G(wo, wi, normal) * fresnel(wo, wh) *
           specular_reflectance / (4 * cos_theta_i * cos_theta_o);
}

real Microfacet::G(const vec3 &wo, const vec3 &wi, const vec3 &normal) const {
    return 1.0 / (1.0 + Lambda(wo, normal) + Lambda(wi, normal));
}

vec3 Microfacet::fresnel(const vec3 &wo, const vec3 &wh) const {
    real cos_theta = glm::dot(wo, wh);
    HARUNOBU_CHECK(cos_theta >= 0. && cos_theta <= 1.,
                   "cos_theta invalid at fresnel!");
    real cos2_theta = cos_theta * cos_theta;
    real sin2_theta = 1. - cos2_theta;
    vec3 eta2 = eta * eta;
    vec3 k2 = k * k;

    vec3 t = eta2 - k2 - sin2_theta;
    vec3 a2_plus_b2 = glm::sqrt(t * t + static_cast<real>(4.) * eta2 * k2);
    vec3 a = glm::sqrt(static_cast<real>(.5) * (a2_plus_b2 + t));
    vec3 a_cos_2 = static_cast<real>(2.) * a * cos_theta;
    vec3 Rs2 = (a2_plus_b2 + cos2_theta - a_cos_2) /
               (a2_plus_b2 + cos2_theta + a_cos_2);
    vec3 Rp2 = Rs2 *
               (cos2_theta * a2_plus_b2 + sin2_theta * sin2_theta -
                a_cos_2 * sin2_theta) /
               (cos2_theta * a2_plus_b2 + sin2_theta * sin2_theta +
                a_cos_2 * sin2_theta);
    return static_cast<real>(.5) * (Rs2 + Rp2);
}

void Microfacet::log_current_status() const {
    HARUNOBU_INFO("Microfacet");
    HARUNOBU_INFO("eta = {}", glm::to_string(eta));
    HARUNOBU_INFO("k = {}", glm::to_string(k));
    HARUNOBU_INFO("specular_reflectance = {}",
                  glm::to_string(specular_reflectance));
}

real Beckmann::D(const vec3 &wh, const vec3 &normal) const {
    real cos_theta = normal_dot(normal, wh);
    HARUNOBU_CHECK(cos_theta > 0.0, "Error computing D at beckmann!");
    real cos2_theta = cos_theta * cos_theta;
    real tan2_theta = (1. - cos2_theta) / cos2_theta;
    real cos4_theta = cos2_theta * cos2_theta;
    real alpha2 = alpha * alpha;

    return std::exp(-tan2_theta / alpha2) / (pi() * alpha2 * cos4_theta);
}

real Beckmann::Lambda(const vec3 &w, const vec3 &normal) const {
    real cos_theta = normal_dot(normal, w);
    HARUNOBU_CHECK(cos_theta > 0.0, "Error computing Lambda at beckmann!");
    real tan_theta = std::sqrt(1.0 - cos_theta * cos_theta) / cos_theta;
    real a = 1.0 / (alpha * tan_theta);
    return .5 * (std::erf(a) - 1 + std::exp(-a * a) / (a * std::sqrt(pi())));
}

void Beckmann::log_current_status() const {
    Microfacet::log_current_status();
    HARUNOBU_INFO("Beckmann");
    HARUNOBU_INFO("alpha = {}", alpha);
}

HARUNOBU_NAMESPACE_END