#include <harunobu/material/diffuse.h>
#include <harunobu/material/material_base.h>
#include <harunobu/material/microfacet.h>

HARUNOBU_NAMESPACE_BEGIN

void MaterialBase::log_current_status() const {
    HARUNOBU_INFO("MaterialBase");
    HARUNOBU_INFO("name = {}", name);
    HARUNOBU_INFO("rgb = {}", glm::to_string(rgb));
}

void MaterialBase::sample(sptr<LocalInfo> linfo) const {
    real phi = rng.random_real(0, 2 * pi());
    real theta = std::acos(std::sqrt(rng.random_real()));
    vec3 basis(std::cos(phi) * std::sin(theta), std::sin(phi) * std::sin(theta),
               std::cos(theta));
    linfo->wi = basis;
}

real MaterialBase::pdf(sptr<LocalInfo> linfo) const {
    real cos_theta_i = linfo->normal_dot(linfo->wi);
    if (cos_theta_i > 0 && linfo->normal_dot(linfo->wo) > 0) {
        return cos_theta_i / pi();
    } else {
        return 0.;
    }
}

sptr<MaterialBase> MaterialBase::factory(std::string name,
                                         ParamSet &param_set) {
    if (name == "diffuse") {
        sptr<Diffuse> material = std::make_shared<Diffuse>();
        material->name = param_set.get<std::string>("id", "");
        material->is_two_sided = param_set.get<bool>("is_two_sided", false);
        material->rgb = param_set.get<vec3>("reflectance", vec3(0, 0, 0));
        return material;
    } else if (name == "roughconductor") {
        auto type = param_set.get<std::string>("distribution", "beckmann");
        if (type == "beckmann") {
            sptr<Beckmann> material = std::make_shared<Beckmann>();
            material->name = param_set.get<std::string>("id", "");
            material->is_two_sided = param_set.get<bool>("is_two_sided", false);
            material->eta = param_set.get<vec3>("eta", vec3(0, 0, 0));
            material->k = param_set.get<vec3>("k", vec3(0, 0, 0));
            material->specular_reflectance =
                param_set.get<vec3>("specularReflectance", vec3(1, 1, 1));
            material->alpha = param_set.get<real>("alpha", 0.01);
            return material;
        } else {
            HARUNOBU_CHECK(false, "Unsupported microfacet material type '{}!",
                           type);
        }
    } else {
        HARUNOBU_CHECK(false, "Unsupported material type '{}'!", name);
    }
}

HARUNOBU_NAMESPACE_END