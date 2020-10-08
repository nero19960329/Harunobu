#include <harunobu/core/image_pipeline.h>

HARUNOBU_NAMESPACE_BEGIN

void ImagePipeline::dump_image(const cv::Mat &raw_image) {
    cv::Mat image = raw_image;

    // perform in float32
    clamp(image, 0.0f, 1.0f);

    image.convertTo(image, CV_8UC3, 255);

    // perform in uint8
    gamma_correction(image, gamma);

    std::string output_name = file_name + "." + file_format;
    HARUNOBU_INFO("Dump into {} ...", output_name);
    cv::imwrite(output_name, image);
    HARUNOBU_INFO("Dump done.");
}

void ImagePipeline::log_current_status() const {
    HARUNOBU_INFO("ImagePipeline");
    HARUNOBU_INFO("gamma = {}", gamma);
    HARUNOBU_INFO("file_format = {}", file_format);
}

void ImagePipeline::clamp(cv::Mat &image, float min_value, float max_value) {
    HARUNOBU_ASSERT(image.type() == CV_32FC3,
                    "Only float32 image could perform clamp!");

    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            auto &rgb = image.at<cv::Vec3f>(i, j);
            rgb[0] = std::min(std::max(rgb[0], min_value), max_value);
            rgb[1] = std::min(std::max(rgb[1], min_value), max_value);
            rgb[2] = std::min(std::max(rgb[2], min_value), max_value);
        }
    }
}

void ImagePipeline::gamma_correction(cv::Mat &image, real gamma) {
    HARUNOBU_ASSERT(image.type() == CV_8UC3,
                    "Only uint8 image could perform gamma corretion!");

    real inverse_gamma = 1.0 / gamma;

    cv::Mat lut(1, 256, CV_8UC1);
    for (int i = 0; i < lut.cols; ++i) {
        lut.at<uchar>(i) = static_cast<uchar>(
            pow(static_cast<real>(i) / 255, inverse_gamma) * 255.0 + .5);
    }

    cv::LUT(image, lut, image);
}

HARUNOBU_NAMESPACE_END