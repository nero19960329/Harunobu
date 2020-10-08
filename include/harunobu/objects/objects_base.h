#pragma once

#include <harunobu/core/common.h>
#include <harunobu/primitive/primitive_base.h>

HARUNOBU_NAMESPACE_BEGIN

class ObjectsBase {
public:
    ObjectsBase() {}

    virtual void add_primitive(sptr<PrimitiveBase> prim);
    virtual bool build() { return true; }

private:
    std::vector<sptr<GeometryBase>> geos;
}; // class ObjectsBase

HARUNOBU_NAMESPACE_END