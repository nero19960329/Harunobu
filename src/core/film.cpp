#include <harunobu/core/film.h>

HARUNOBU_NAMESPACE_BEGIN

Film::Film(int width_, int height_) :
    width(width_), height(height_) {
    image = cv::Mat::zeros(height_, width_, CV_32FC3);
    recon_weight_sum = cv::Mat::zeros(height_, width_, CV_32FC1);
}

void Film::add_sample(const cv::Vec3f &bgr, real i, real j) {
    real rx = rfilter->rx, ry = rfilter->ry;
    int x_min = std::ceil(i - rx), y_min = std::ceil(j - ry);
    int x_max = std::ceil(i + rx), y_max = std::ceil(j + ry);
    x_min = std::max(0, x_min);
    y_min = std::max(0, y_min);
    x_max = std::min(x_max, width);
    y_max = std::min(y_max, height);
    for (int x = x_min; x < x_max; ++x) {
        for (int y = y_min; y < y_max; ++y) {
            real recon_weight = rfilter->eval(i - x, j - y);
            image.at<cv::Vec3f>(y, x) += bgr * recon_weight;
            recon_weight_sum.at<float>(y, x) += recon_weight;
        }
    }
    //getchar();
}

cv::Mat Film::make_image() {
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            image.at<cv::Vec3f>(j, i) /= recon_weight_sum.at<float>(j, i);
        }
    }
    return image;
}

void Film::log_current_status() const {
    HARUNOBU_INFO("Film");
    HARUNOBU_INFO("height = {}, width = {}", height, width);
}

HARUNOBU_NAMESPACE_END