#include <harunobu/core/image_pipeline.h>

HARUNOBU_NAMESPACE_BEGIN

void ImagePipeline::dump_image(const cv::Mat& raw_image) {
    cv::Mat image;
    raw_image.convertTo(image, CV_8UC3, 255);
    cv::imwrite("./scene.png", image);
}

HARUNOBU_NAMESPACE_END