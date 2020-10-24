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
        return std::max(zero, rx - std::abs(i)) *
               std::max(zero, ry - std::abs(j));
    }
}; // class TentFilter

HARUNOBU_NAMESPACE_END