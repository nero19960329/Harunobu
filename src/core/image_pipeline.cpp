#include <harunobu/core/image_pipeline.h>

HARUNOBU_NAMESPACE_BEGIN

void ImagePipeline::dump_image(const cv::Mat &raw_image) {
    cv::Mat image = raw_image;

    // perform in float32
    gamma_correction(image, gamma);
    clamp(image, 0.0f, 1.0f);

    image.convertTo(image, CV_8UC3, 255);

    // perform in uint8

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

#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            image.at<cv::Vec3f>(j, i)[0] =
                pow(image.at<cv::Vec3f>(j, i)[0], 1.0 / gamma);
            image.at<cv::Vec3f>(j, i)[1] =
                pow(image.at<cv::Vec3f>(j, i)[1], 1.0 / gamma);
            image.at<cv::Vec3f>(j, i)[2] =
                pow(image.at<cv::Vec3f>(j, i)[2], 1.0 / gamma);
        }
    }
}

HARUNOBU_NAMESPACE_END