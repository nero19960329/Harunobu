#include <unordered_map>

#include <harunobu/primitive/primitive_base.h>
#include <harunobu/primitive/rect.h>

namespace harunobu {

PrimitiveBase::PrimitiveBase(sptr<MaterialBase> material_,
                             const mat4 &trans_mat)
    : material(material_) {
    make_geos(trans_mat);
}

/*sptr<PrimitiveBase> PrimitiveBase::factory(std::string name) {
    std::unordered_map<std::string, sptr<PrimitiveBase>> fact;
}*/

} // namespace harunobu