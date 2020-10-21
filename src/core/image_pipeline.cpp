#include <fstream>

#include <harunobu/core/image_pipeline.h>

#include <png.hpp>

HARUNOBU_NAMESPACE_BEGIN

void ImagePipeline::dump_image(sptr<Image<real>> raw_image,
                               std::string output_path) {
    // perform in float32
    gamma_correction(raw_image, gamma);
    clamp(raw_image, 0.0f, 1.0f);

    auto image = convert(raw_image);

    // perform in uint8

    HARUNOBU_INFO("Dump into {} ...", output_path);
    auto extension = output_path.substr(output_path.size() - 3, 3);
    HARUNOBU_CHECK(output_path.size() > 3 && extension == "png",
                   "Invalid extension {}, only png is supported!", extension)
    save_image(image, output_path);
    HARUNOBU_INFO("Dump done.");
}

void ImagePipeline::log_current_status() const {
    HARUNOBU_INFO("ImagePipeline");
    HARUNOBU_INFO("gamma = {}", gamma);
    HARUNOBU_INFO("file_format = {}", file_format);
}

void ImagePipeline::clamp(sptr<Image<real>> image, real min_value,
                          real max_value) {
    for (size_t i = 0; i < image->width; ++i) {
        for (size_t j = 0; j < image->height; ++j) {
            for (size_t k = 0; k < image->channel; ++k) {
                image->at(k, j, i) = std::min(
                    std::max(image->at(k, j, i), min_value), max_value);
            }
        }
    }
}

void ImagePipeline::gamma_correction(sptr<Image<real>> image, real gamma) {
    for (size_t i = 0; i < image->width; ++i) {
        for (size_t j = 0; j < image->height; ++j) {
            for (size_t k = 0; k < image->channel; ++k) {
                image->at(k, j, i) = pow(image->at(k, j, i), 1.0 / gamma);
            }
        }
    }
}

sptr<Image<unsigned char>> ImagePipeline::convert(sptr<Image<real>> image) {
    sptr<Image<unsigned char>> output = std::make_shared<Image<unsigned char>>(
        std::array<size_t, 3>{image->channel, image->height, image->width});
    for (size_t i = 0; i < image->width; ++i) {
        for (size_t j = 0; j < image->height; ++j) {
            for (size_t k = 0; k < image->channel; ++k) {
                output->at(k, j, i) = static_cast<unsigned char>(
                    std::round(image->at(k, j, i) * 255.0));
            }
        }
    }
    return output;
}

void ImagePipeline::save_image(sptr<Image<unsigned char>> image,
                               std::string output_name) {
    png::image<png::rgb_pixel> image_png(image->width, image->height);
    for (size_t i = 0; i < image->width; ++i) {
        for (size_t j = 0; j < image->height; ++j) {
            image_png[j][i] = png::rgb_pixel(
                image->at(0, j, i), image->at(1, j, i), image->at(2, j, i));
        }
    }
    image_png.write(output_name);
}

HARUNOBU_NAMESPACE_END