#pragma once

#include <harunobu/core/common.h>
#include <harunobu/primitive/primitive_base.h>

namespace harunobu {

class ObjectsBase {
public:
    ObjectsBase() {}

    virtual void add_primitive(sptr<PrimitiveBase> prim);
    virtual bool build() { return true; }

private:
    std::vector<sptr<GeometryBase>> geos;
}; // class ObjectsBase

} // namespace harunobu