#include <limits>

#include <harunobu/objects/objects_base.h>

HARUNOBU_NAMESPACE_BEGIN

void ObjectsBase::add_primitive(sptr<PrimitiveBase> prim) {
    geos.insert(geos.end(), prim->geos.begin(), prim->geos.end());
    prim->idx = prims.size();
    prims.push_back(prim);
}

sptr<Intersect> ObjectsBase::ray_intersect(const Ray &ray, bool &is_intersect) {
    real t = std::numeric_limits<real>::max();
    sptr<Intersect> nearest_intersect = std::make_shared<Intersect>();
    for (auto &geo : geos) {
        bool is_intersect;
        auto intersect = geo->ray_intersect(ray, is_intersect);
        if (is_intersect && intersect->ray_step < t) {
            t = intersect->ray_step;
            nearest_intersect = intersect;
        }
    }

    is_intersect = (t != std::numeric_limits<real>::max());
    return nearest_intersect;
}

HARUNOBU_NAMESPACE_END