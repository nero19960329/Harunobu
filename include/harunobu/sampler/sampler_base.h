#pragma once

#include <harunobu/core/common.h>

HARUNOBU_NAMESPACE_BEGIN

class SamplerBase {
public:
    int sample_count;

public:
    SamplerBase() : pixel_index(0) {}

    virtual void next_pixel() { pixel_index++; }

    virtual real next_1D() const = 0;
    virtual vec2 next_2D() const = 0;

protected:
    size_t pixel_index;
}; // class SamplerBase

HARUNOBU_NAMESPACE_END