#include <iostream>
#include <string>

#include <harunobu/core/common.h>
#include <harunobu/io/mitsuba_reader.h>

int main(int argc, char *argv[]) {
    harunobu::initialize_env();

    HARUNOBU_CHECK(
        argc == 3,
        "Invalid argument! Usage: harunobu <scene_xml path> <output path>");

    harunobu::MitsubaReader scene_reader;
    auto scene = scene_reader.load(std::string(argv[1]));
    scene->render(std::string(argv[2]));

    return 0;
}