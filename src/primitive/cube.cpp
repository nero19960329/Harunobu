#include <harunobu/geometry/tri.h>
#include <harunobu/primitive/cube.h>

namespace harunobu {

Cube::Cube(sptr<MaterialBase> material_) : PrimitiveBase(material_) {}

void Cube::make_geos(const mat4 &trans_mat) {}

} // namespace harunobu