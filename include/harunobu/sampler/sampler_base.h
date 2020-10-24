#pragma once

#include <harunobu/core/common.h>
#include <harunobu/core/param_set.h>

HARUNOBU_NAMESPACE_BEGIN

class SamplerBase {
public:
    int sample_count;

public:
    SamplerBase(int sample_count_)
        : sample_count(sample_count_), sample_idx(0) {}

    virtual sptr<SamplerBase> copy() = 0;

    virtual void next_pixel() { sample_idx = 0; }

    virtual real next_1D() = 0;
    virtual vec2 next_2D() = 0;

    static sptr<SamplerBase> factory(std::string name, ParamSet &param_set);

protected:
    int sample_idx;
}; // class SamplerBase

HARUNOBU_NAMESPACE_END