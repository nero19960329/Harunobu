#pragma once

#include <harunobu/material/material_base.h>

HARUNOBU_NAMESPACE_BEGIN

class Diffuse : public MaterialBase {
public:
    Diffuse() : MaterialBase() {}

    virtual vec3 f(sptr<LocalInfo> linfo) const override {
        return rgb / static_cast<real>(pi());
    }
}; // class Diffuse

HARUNOBU_NAMESPACE_END