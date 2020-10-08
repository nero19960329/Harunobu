#pragma once

#include <harunobu/core/common.h>

#include <opencv2/opencv.hpp>

HARUNOBU_NAMESPACE_BEGIN

class ImagePipeline {
public:
    std::string file_format;
    float gamma;

public:
    ImagePipeline() {}

    void dump_image(const cv::Mat& raw_image);
};

HARUNOBU_NAMESPACE_END