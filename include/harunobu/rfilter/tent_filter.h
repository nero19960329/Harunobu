#pragma once

#include <harunobu/rfilter/rfilter_base.h>

HARUNOBU_NAMESPACE_BEGIN

class TentFilter : public RFilterBase {
public:
    TentFilter() : RFilterBase() {
        rx = 1.0;
        ry = 1.0;
    }

    virtual real eval(real i, real j) const {
        return std::max(static_cast<real>(0.), rx - std::abs(i)) *
            std::max(static_cast<real>(0.), ry - std::abs(j));
    }
}; // class TentFilter

HARUNOBU_NAMESPACE_END