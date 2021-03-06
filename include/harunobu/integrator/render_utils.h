#pragma once

#include <harunobu/core/basic.h>
#include <harunobu/core/common.h>
#include <harunobu/core/pmf.h>
#include <harunobu/geometry/geometry_base.h>
#include <harunobu/objects/objects_base.h>
#include <harunobu/primitive/primitive_base.h>

HARUNOBU_NAMESPACE_BEGIN

class RenderUtils {
public:
    static sptr<SampleInfo>
    light_sample(sptr<Intersect> intersect,
                 const std::vector<sptr<SampleInfo>> &sinfo_vec, PMF &pmf);

    static vec3 get_direct_radiance(std::vector<sptr<PrimitiveBase>> lights,
                                    sptr<Intersect> intersect,
                                    sptr<ObjectsBase> objects,
                                    int light_sample_num, int bsdf_sample_num);
}; // class RenderUtils

HARUNOBU_NAMESPACE_END