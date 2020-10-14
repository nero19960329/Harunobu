#include <harunobu/primitive/cube.h>
#include <harunobu/primitive/primitive_base.h>
#include <harunobu/primitive/rect.h>
#include <harunobu/primitive/sphere.h>

HARUNOBU_NAMESPACE_BEGIN

PrimitiveBase::PrimitiveBase(sptr<MaterialBase> material_)
    : material(material_), emit_radiance(0, 0, 0), area(0.0) {}

sptr<PrimitiveBase> PrimitiveBase::factory(std::string name,
                                           sptr<MaterialBase> material,
                                           ParamSet &param_set) {
    if (name == "rectangle") {
        auto prim = std::make_shared<Rect>(material);
        prim->init(param_set);
        return prim;
    } else if (name == "cube") {
        auto prim = std::make_shared<Cube>(material);
        prim->init(param_set);
        return prim;
    } else if (name == "sphere") {
        auto prim = std::make_shared<Sphere>(material);
        prim->init(param_set);
        return prim;
    } else {
        HARUNOBU_CHECK(false, "Unsupported shape '{}'!", name);
    }
}

void PrimitiveBase::make_area() {
    area = 0.0;
    for (const auto &geo : geos) {
        area += geo->area;
    }
}

HARUNOBU_NAMESPACE_END