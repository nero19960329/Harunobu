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

inline real vec3_max_elem(const vec3 &v) {
    return std::max(std::max(v.x, v.y), v.z);
}

sptr<SampleInfo> PrimitiveBase::light_sample(sptr<Intersect> intersect) {
    if (geos.size() == 1) {
        return geos[0]->light_sample(intersect);
    }

    std::vector<real> pdf(geos.size());
    std::vector<sptr<SampleInfo>> sinfo_vec(geos.size());
    real sum = 0;
    for (size_t i = 0; i < geos.size(); ++i) {
        auto sinfo = geos[i]->light_sample(intersect);
        sinfo_vec[i] = sinfo;
        pdf[i] = vec3_max_elem(
            RenderUtils::get_direct_radiance_wo_shadow_test(intersect, sinfo));
        sum += pdf[i];
    }

    if (sum != 0.0) {
        for (auto &w : pdf) {
            w /= sum;
        }
    } else {
        real w_avg = 1.0 / pdf.size();
        for (auto &w : pdf) {
            w = w_avg;
        }
    }

    std::vector<real> cdf(geos.size() + 1);
    cdf[0] = 0;
    for (size_t i = 1; i <= pdf.size(); ++i) {
        cdf[i] = pdf[i - 1] + cdf[i - 1];
    }
    real random = rng.random_real();
    size_t idx;
    for (idx = 1; idx < cdf.size(); ++idx) {
        if (random <= cdf[idx]) {
            break;
        }
    }

    auto sinfo = sinfo_vec[idx - 1];
    sinfo->pdf *= pdf[idx - 1];
    return sinfo;
}

HARUNOBU_NAMESPACE_END