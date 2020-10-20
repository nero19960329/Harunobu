#include <fstream>
#include <harunobu/core/image_pipeline.h>

HARUNOBU_NAMESPACE_BEGIN

void ImagePipeline::dump_image(sptr<Image<real>> raw_image) {
    // perform in float32
    gamma_correction(raw_image, gamma);
    clamp(raw_image, 0.0f, 1.0f);

    auto image = convert(raw_image);

    // perform in uint8

    std::string output_name = file_name + ".ppm";
    HARUNOBU_INFO("Dump into {} ...", output_name);
    
    std::ofstream fout(output_name, std::ios_base::out | std::ios_base::binary);
    fout << "P6\n" << image->width << " " << image->height << std::endl << "255" << std::endl;
    for (size_t j = 0; j < image->height; ++j) {
        for (size_t i = 0; i < image->width; ++i) {
            fout << (char) (image->at(0, j, i)) << (char) (image->at(1, j, i)) << (char) (image->at(2, j, i));
        }
    }
    fout.close();

    HARUNOBU_INFO("Dump done.");
}

void ImagePipeline::log_current_status() const {
    HARUNOBU_INFO("ImagePipeline");
    HARUNOBU_INFO("gamma = {}", gamma);
    HARUNOBU_INFO("file_format = {}", file_format);
}

void ImagePipeline::clamp(sptr<Image<real>> image, real min_value, real max_value) {
    for (size_t i = 0; i < image->width; ++i) {
        for (size_t j = 0; j < image->height; ++j) {
            for (size_t k = 0; k < image->channel; ++k) {
                image->at(k, j, i) = std::min(std::max(image->at(k, j, i), min_value), max_value);
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
    sptr<Image<unsigned char>> output = std::make_shared<Image<unsigned char>>(std::array<size_t, 3>{ image->channel, image->height, image->width });
    for (size_t i = 0; i < image->width; ++i) {
        for (size_t j = 0; j < image->height; ++j) {
            for (size_t k = 0; k < image->channel; ++k) {
                output->at(k, j, i) = static_cast<unsigned char>(std::round(image->at(k, j, i) * 255.0));
            }
        }
    }
    return output;
}

HARUNOBU_NAMESPACE_END