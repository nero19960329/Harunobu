#pragma once

#include <opencv2/opencv.hpp>

#include <harunobu/core/common.h>
#include <harunobu/rfilter/rfilter_base.h>

HARUNOBU_NAMESPACE_BEGIN

class Film {
public:
    int width, height;
    sptr<RFilterBase> rfilter;

private:
    cv::Mat image;
    cv::Mat recon_weight_sum;

public:
    Film(int width_, int height_);

    void add_sample(const cv::Vec3f &rgb, real i, real j);
    cv::Mat make_image();

    void log_current_status() const;
}; // class Film

HARUNOBU_NAMESPACE_END