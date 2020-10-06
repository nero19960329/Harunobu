#include <iostream>
#include <string>

#include <harunobu/core/common.h>
#include <harunobu/io/mitsuba_reader.h>

int main(int argc, char *argv[]) {
    harunobu::initialize_env();

    HARUNOBU_CHECK(argc == 2,
                   "Invalid argument! Usage: harunobu <scene_xml path>");

    harunobu::MitsubaReader scene_reader;
    scene_reader.load(std::string(argv[1]));

    return 0;
}