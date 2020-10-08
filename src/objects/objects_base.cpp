#include <harunobu/objects/objects_base.h>

HARUNOBU_NAMESPACE_BEGIN

void ObjectsBase::add_primitive(sptr<PrimitiveBase> prim) {
    geos.insert(geos.end(), prim->geos.begin(), prim->geos.end());
}

HARUNOBU_NAMESPACE_END