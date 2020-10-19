#include <harunobu/integrator/render_utils.h>
#include <harunobu/primitive/cube.h>
#include <harunobu/primitive/primitive_base.h>
#include <harunobu/primitive/rect.h>
#include <harunobu/primitive/sphere.h>

HARUNOBU_NAMESPACE_BEGIN

PrimitiveBase::PrimitiveBase(sptr<MaterialBase> material_)
    : material(material_), emit_radiance(0, 0, 0), area(0.0) {}

sptr<PrimitiveBase> PrimitiveBase::factory(std::string name,
                                           sptr<MaterialBase> material,
                                           ParamSet &param_set) {
    if (name == "rectangle") {
        auto prim = std::make_shared<Rect>(material);
        prim->init(param_set);
        return prim;
    } else if (name == "cube") {
        auto prim = std::make_shared<Cube>(material);
        prim->init(param_set);
        return prim;
    } else if (name == "sphere") {
        auto prim = std::make_shared<Sphere>(material);
        prim->init(param_set);
        return prim;
    } else {
        HARUNOBU_CHECK(false, "Unsupported shape '{}'!", name);
    }
}

void PrimitiveBase::make_area() {
    area = 0.0;
    for (const auto &geo : geos) {
        area += geo->area;
    }
}

sptr<SampleInfo> PrimitiveBase::random_sample() {
    size_t random_idx = rng.random_idx(0, geos.size() - 1);
    auto sinfo = geos[random_idx]->random_sample();
    sinfo->pdf /= geos.size();
    return sinfo;
}

sptr<SampleInfo> PrimitiveBase::light_sample(sptr<Intersect> intersect,
                                             PMF &pmf) {
    int n = geos.size();
    std::vector<sptr<SampleInfo>> sinfo_vec(n);
    for (size_t i = 0; i < n; ++i) {
        auto sinfo = geos[i]->light_sample(intersect);
        sinfo_vec[i] = sinfo;
    }
    return RenderUtils::light_sample(intersect, sinfo_vec, pmf);
}

HARUNOBU_NAMESPACE_END