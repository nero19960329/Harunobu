#pragma once

#include <harunobu/primitive/primitive_base.h>

namespace harunobu {

class Cube : public PrimitiveBase {
public:
    Cube(sptr<MaterialBase> material_);

private:
    virtual void make_geos(const mat4 &trans_mat) override;
}; // class Cube

}; // namespace harunobu