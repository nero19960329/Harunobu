#pragma once

#include <harunobu/core/common.h>

HARUNOBU_NAMESPACE_BEGIN

class RFilterBase {
public:
    real rx, ry;

public:
    RFilterBase() {}

    virtual real eval(real i, real j) const = 0;

    static sptr<RFilterBase> factory(std::string name);
}; // class RFilterBase

HARUNOBU_NAMESPACE_END