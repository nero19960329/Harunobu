#pragma once

#include <harunobu/primitive/primitive_base.h>

HARUNOBU_NAMESPACE_BEGIN

class Rect : public PrimitiveBase {
public:
    Rect(sptr<MaterialBase> material_);

    virtual void make_geos(const mat4 &trans_mat) override;
    virtual void log_current_status() const override;
}; // class Rect
}
; // HARUNOBU_NAMESPACE_BEGIN