#include <harunobu/geometry/sphere_geo.h>
#include <harunobu/primitive/sphere.h>

HARUNOBU_NAMESPACE_BEGIN

Sphere::Sphere(sptr<MaterialBase> material_) : PrimitiveBase(material_) {}

void Sphere::init(ParamSet &param_set) {
    geos.clear();
    vec3 center = param_set.get<vec3>("center", vec3(0, 0, 0));
    real radius = param_set.get<real>("radius", 1.);
    auto sphere_geo = std::make_shared<SphereGeo>(this, center, radius);
    sphere_geo->idx = 0;
    geos.push_back(sphere_geo);
    make_area();
}

void Sphere::log_current_status() const {
    HARUNOBU_INFO("Sphere");
    material->log_current_status();
    HARUNOBU_INFO("emit_radiance = {}", glm::to_string(emit_radiance));
    geos[0]->log_current_status();
}

HARUNOBU_NAMESPACE_END