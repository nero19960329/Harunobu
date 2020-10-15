#pragma once

#include <harunobu/primitive/primitive_base.h>

HARUNOBU_NAMESPACE_BEGIN

class Sphere : public PrimitiveBase {
public:
    Sphere(sptr<MaterialBase> material_);

    virtual void log_current_status() const override;

    virtual void init(ParamSet &param_set) override;
}; // class Sphere

HARUNOBU_NAMESPACE_END