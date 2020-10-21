#pragma once

#include <harunobu/geometry/tri.h>
#include <harunobu/primitive/primitive_base.h>

HARUNOBU_NAMESPACE_BEGIN

class Cube : public PrimitiveBase {
public:
    Cube(sptr<MaterialBase> material_);

    virtual void init(ParamSet &param_set) override;

    virtual void log_current_status() const override;

private:
    std::pair<sptr<Tri>, sptr<Tri>>
    make_rect_geos(const mat4 &trans_mat, const std::array<vec3, 4> &vertices,
                   const vec3 &normal);
    void make_geos(const mat4 &trans_mat);
}; // class Cube
}
; // HARUNOBU_NAMESPACE_BEGIN