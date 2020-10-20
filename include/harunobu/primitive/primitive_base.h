#pragma once

#include <string>
#include <vector>

#include <harunobu/core/common.h>
#include <harunobu/core/param_set.h>
#include <harunobu/core/pmf.h>
#include <harunobu/geometry/geometry_base.h>
#include <harunobu/material/material_base.h>

HARUNOBU_NAMESPACE_BEGIN

class PrimitiveBase {
public:
    sptr<MaterialBase> material;
    vec3 emit_radiance;
    std::vector<sptr<GeometryBase>> geos;
    size_t idx;

    real area;
    real light_sampling_weight;

public:
    PrimitiveBase(sptr<MaterialBase> material_);

    virtual void log_current_status() const = 0;

    static sptr<PrimitiveBase>
    factory(std::string name, sptr<MaterialBase> material, ParamSet &param_set);

    virtual void init(ParamSet &param_set) {
        make_area();
        light_sampling_weight = param_set.get<real>("light_sampling_weight", 1.);
    }
    virtual sptr<SampleInfo> random_sample();
    virtual sptr<SampleInfo> light_sample(sptr<Intersect> intersect, PMF &pmf);

protected:
    void make_area();
}; // class PrimitiveBase

HARUNOBU_NAMESPACE_END