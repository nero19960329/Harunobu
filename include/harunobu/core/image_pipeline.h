#pragma once

#include <harunobu/core/common.h>
#include <harunobu/core/image.h>

HARUNOBU_NAMESPACE_BEGIN

class ImagePipeline {
public:
    std::string file_name;
    std::string file_format;
    real gamma;

public:
    ImagePipeline() {}

    void dump_image(sptr<Image<real>> raw_image);
    void log_current_status() const;

private:
    void clamp(sptr<Image<real>> image, real min_value, real max_value);
    void gamma_correction(sptr<Image<real>> image, real gamma);
    sptr<Image<unsigned char>> convert(sptr<Image<real>> image);
};

HARUNOBU_NAMESPACE_END