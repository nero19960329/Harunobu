#include <harunobu/geometry/tri.h>
#include <harunobu/primitive/rect.h>

namespace harunobu {

Rect::Rect(sptr<MaterialBase> material_, const mat4 &trans_mat)
    : PrimitiveBase(material_, trans_mat) {}

void Rect::make_geos(const mat4 &trans_mat) {
    sptr<Tri> tri1 = std::make_shared<Tri>(this);
    tri1->vertices[0] = vec3(-1.0, 1.0, 0.0);
    tri1->vertices[1] = vec3(-1.0, -1.0, 0.0);
    tri1->vertices[2] = vec3(1.0, -1.0, 0.0);
    tri1->normals[0] = tri1->normals[1] = tri1->normals[2] =
        vec3(0.0, 0.0, 1.0);
    tri1->do_transform(trans_mat);
    sptr<Tri> tri2 = std::make_shared<Tri>(this);
    tri2->vertices[0] = vec3(1.0, -1.0, 0.0);
    tri2->vertices[1] = vec3(1.0, 1.0, 0.0);
    tri2->vertices[2] = vec3(-1.0, 1.0, 0.0);
    tri2->normals[0] = tri2->normals[1] = tri2->normals[2] =
        vec3(0.0, 0.0, 1.0);
    tri2->do_transform(trans_mat);
    geos.push_back(tri1);
    geos.push_back(tri2);
}

} // namespace harunobu