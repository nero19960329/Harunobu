#include <harunobu/primitive/cube.h>
#include <harunobu/primitive/primitive_base.h>
#include <harunobu/primitive/rect.h>

namespace harunobu {

PrimitiveBase::PrimitiveBase(sptr<MaterialBase> material_)
    : material(material_) {}

sptr<PrimitiveBase> PrimitiveBase::factory(std::string name,
                                           sptr<MaterialBase> material,
                                           const mat4 &trans_mat) {
    if (name == "rectangle") {
        auto prim = std::make_shared<Rect>(material);
        prim->make_geos(trans_mat);
        return prim;
    } else if (name == "cube") {
        auto prim = std::make_shared<Cube>(material);
        prim->make_geos(trans_mat);
        return prim;
    } else {
        HARUNOBU_CHECK(false, "Unsupported shape '{}'!", name)
    }
}

} // namespace harunobu