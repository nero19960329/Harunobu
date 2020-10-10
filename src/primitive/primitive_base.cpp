#include <harunobu/primitive/cube.h>
#include <harunobu/primitive/primitive_base.h>
#include <harunobu/primitive/rect.h>

HARUNOBU_NAMESPACE_BEGIN

PrimitiveBase::PrimitiveBase(sptr<MaterialBase> material_)
    : material(material_), emit_radiance(0, 0, 0), area(0.0) {}

sptr<PrimitiveBase> PrimitiveBase::factory(std::string name,
                                           sptr<MaterialBase> material,
                                           const mat4 &trans_mat) {
    if (name == "rectangle") {
        auto prim = std::make_shared<Rect>(material);
        prim->make_geos(trans_mat);
        prim->get_area();
        return prim;
    } else if (name == "cube") {
        auto prim = std::make_shared<Cube>(material);
        prim->make_geos(trans_mat);
        prim->get_area();
        return prim;
    } else {
        HARUNOBU_CHECK(false, "Unsupported shape '{}'!", name);
    }
}

real PrimitiveBase::get_area() {
    area = 0.0;
    for (const auto &geo : geos) {
        area += geo->area;
    }
    return area;
}

HARUNOBU_NAMESPACE_END