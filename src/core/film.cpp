#include <harunobu/core/film.h>

HARUNOBU_NAMESPACE_BEGIN

Film::Film(int width_, int height_) : width(width_), height(height_) {
    image = std::make_shared<Image<real>>(std::array<size_t, 3>{ 3, height, width });
    recon_weight_sum = std::make_shared<Image<real>>(std::array<size_t, 3>{ 1, height, width });
}

void Film::add_sample(const vec3 &rgb, real i, real j) {
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
            for (int k = 0; k < 3; ++k) {
                image->at(k, j, i) += rgb[k] * recon_weight;
            }
            recon_weight_sum->at(0, j, i) += recon_weight;
        }
    }
}

sptr<Image<real>> Film::make_image() {
    for (int i = 0; i < image->width; ++i) {
        for (int j = 0; j < image->height; ++j) {
            for (int k = 0; k < image->channel; ++k) {
                image->at(k, j, i) /= recon_weight_sum->at(0, j, i);
            }
        }
    }
    return image;
}

void Film::log_current_status() const {
    HARUNOBU_INFO("Film");
    HARUNOBU_INFO("height = {}, width = {}", height, width);
}

HARUNOBU_NAMESPACE_END