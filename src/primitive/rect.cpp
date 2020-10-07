#include <harunobu/geometry/tri.h>
#include <harunobu/primitive/rect.h>

namespace harunobu {

Rect::Rect(sptr<MaterialBase> material_) : PrimitiveBase(material_) {}

void Rect::make_geos(const mat4 &trans_mat) {
    geos.clear();
    sptr<Tri> tri1 = std::make_shared<Tri>(
        this,
        std::array<vec3, 3>{vec3(-1, 1, 0), vec3(-1, -1, 0), vec3(1, -1, 0)},
        std::array<vec3, 3>{vec3(0, 0, 1), vec3(0, 0, 1), vec3(0, 0, 1)});
    sptr<Tri> tri2 = std::make_shared<Tri>(
        this,
        std::array<vec3, 3>{vec3(1, -1, 0), vec3(1, 1, 0), vec3(-1, 1, 0)},
        std::array<vec3, 3>{vec3(0, 0, 1), vec3(0, 0, 1), vec3(0, 0, 1)});
    tri1->do_transform(trans_mat);
    tri2->do_transform(trans_mat);
    geos.push_back(tri1);
    geos.push_back(tri2);
}

void Rect::log_current_status() const {
    HARUNOBU_INFO("Rect");
    material->log_current_status();
    for (const auto &geo : geos) {
        geo->log_current_status();
    }
}

} // namespace harunobu