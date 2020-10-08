#pragma once

#include <harunobu/core/common.h>

#include <opencv2/opencv.hpp>

HARUNOBU_NAMESPACE_BEGIN

class ImagePipeline {
public:
    std::string file_name;
    std::string file_format;
    real gamma;

public:
    ImagePipeline() {}

    void dump_image(const cv::Mat &raw_image);
    void log_current_status() const;

private:
    void clamp(cv::Mat &image, float min_value, float max_value);
    void gamma_correction(cv::Mat &image, real gamma);
};

HARUNOBU_NAMESPACE_END