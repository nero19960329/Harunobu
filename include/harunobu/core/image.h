#pragma once

#include <sstream>

#include <harunobu/core/common.h>

HARUNOBU_NAMESPACE_BEGIN

template <typename T> class Image {
private:
    uptr<T[]> data;

public:
    size_t width, height, channel;

public:
    Image(std::array<size_t, 3> shape)
        : data(new T[shape[0] * shape[1] * shape[2]]) {
        channel = shape[0];
        height = shape[1];
        width = shape[2];
        size_t elem_num = shape[0] * shape[1] * shape[2];
        std::fill_n(data.get(), elem_num, 0);
    }

    T &at(int k, int j, int i) { return data.get()[idx(k, j, i)]; }

    T at(int k, int j, int i) const { return data.get()[idx(k, j, i)]; }

    std::string to_string() {
        std::ostringstream oss;
        oss << "[\n";
        for (size_t i = 0; i < width; ++i) {
            oss << "[ ";
            for (size_t j = 0; j < height; ++j) {
                oss << "( ";
                for (size_t k = 0; k < channel; ++k) {
                    oss << at(k, j, i) << " ";
                }
                oss << ") ";
            }
            oss << "],";
        }
        oss << "\n]";
        return oss.str();
    }

private:
    size_t idx(int k, int j, int i) {
        return k + j * channel + i * channel * height;
    }
}; // class Image

HARUNOBU_NAMESPACE_END