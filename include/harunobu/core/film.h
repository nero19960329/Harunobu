#pragma once

#include <harunobu/core/common.h>
#include <harunobu/core/image.h>
#include <harunobu/rfilter/rfilter_base.h>

HARUNOBU_NAMESPACE_BEGIN

class Film {
public:
    size_t width, height;
    sptr<RFilterBase> rfilter;

private:
    sptr<Image<real>> image;
    sptr<Image<real>> recon_weight_sum;

public:
    Film(size_t width_, size_t height_);

    void add_sample(const vec3 &rgb, real i, real j);
    sptr<Image<real>> make_image();

    void log_current_status() const;
}; // class Film

HARUNOBU_NAMESPACE_END