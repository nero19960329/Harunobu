#pragma once

#include <harunobu/rfilter/rfilter_base.h>

HARUNOBU_NAMESPACE_BEGIN

class BoxFilter : public RFilterBase {
public:
    BoxFilter() : RFilterBase() {
        rx = 0.5;
        ry = 0.5;
    }

    virtual real eval(real i, real j) const { return 1.; }
}; // class BoxFilter

HARUNOBU_NAMESPACE_END