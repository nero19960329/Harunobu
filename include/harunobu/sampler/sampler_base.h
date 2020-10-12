#pragma once

#include <harunobu/core/common.h>

HARUNOBU_NAMESPACE_BEGIN

class SamplerBase {
public:
    int sample_count;

public:
    SamplerBase() {}

    virtual vec2 sample_screen_coor(int i, int j) const = 0;
}; // class SamplerBase

HARUNOBU_NAMESPACE_END