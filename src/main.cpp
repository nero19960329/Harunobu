#include <iostream>
#include <string>

#include <harunobu/core/common.h>
#include <harunobu/io/mitsuba_reader.h>

#include <opencv2/opencv.hpp>

int main(int argc, char *argv[]) {
    harunobu::initialize_env();

    HARUNOBU_CHECK(argc == 2,
                   "Invalid argument! Usage: harunobu <scene_xml path>");

    harunobu::MitsubaReader scene_reader;
    auto scene = scene_reader.load(std::string(argv[1]));
    auto image = scene->render();

    cv::imwrite("./scene.png", image);
    return 0;
}