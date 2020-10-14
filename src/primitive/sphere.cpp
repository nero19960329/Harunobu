#include <harunobu/geometry/sphere_geo.h>
#include <harunobu/primitive/sphere.h>

HARUNOBU_NAMESPACE_BEGIN

Sphere::Sphere(sptr<MaterialBase> material_) : PrimitiveBase(material_) {}

void Sphere::init(ParamSet &param_set) {
    geos.clear();
    vec3 center = param_set.get<vec3>("center", vec3(0, 0, 0));
    real radius = param_set.get<real>("radius", 1.);
    geos.push_back(std::make_shared<SphereGeo>(this, center, radius));
    make_area();
}

vec3 Sphere::random_sample() { return geos[0]->random_sample(); }

void Sphere::log_current_status() const {
    HARUNOBU_INFO("Sphere");
    material->log_current_status();
    HARUNOBU_INFO("emit_radiance = {}", glm::to_string(emit_radiance));
    geos[0]->log_current_status();
}

HARUNOBU_NAMESPACE_END