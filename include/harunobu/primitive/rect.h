#pragma once

#include <harunobu/primitive/primitive_base.h>

namespace harunobu {

class Rect : public PrimitiveBase {
public:
    Rect(sptr<MaterialBase> material_, const mat4 &trans_mat);

private:
    virtual void make_geos(const mat4 &trans_mat) override;
}; // class Rect

}; // namespace harunobu