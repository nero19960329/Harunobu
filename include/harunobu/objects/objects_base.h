#pragma once

#include <harunobu/core/basic.h>
#include <harunobu/core/common.h>
#include <harunobu/primitive/primitive_base.h>

HARUNOBU_NAMESPACE_BEGIN

class ObjectsBase {
public:
    std::vector<sptr<GeometryBase>> geos;
    std::vector<sptr<PrimitiveBase>> prims;

public:
    ObjectsBase() {}

    virtual void add_primitive(sptr<PrimitiveBase> prim);
    virtual bool build() { return true; }

    virtual sptr<Intersect> ray_intersect(const Ray &ray, bool &is_intersect);
}; // class ObjectsBase

HARUNOBU_NAMESPACE_END