#include <harunobu/objects/objects_base.h>

namespace harunobu {

void ObjectsBase::add_primitive(sptr<PrimitiveBase> prim) {
    geos.insert(geos.end(), prim->geos.begin(), prim->geos.end());
}

} // namespace harunobu