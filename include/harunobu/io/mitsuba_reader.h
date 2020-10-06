#pragma once

#include <harunobu/core/camera.h>
#include <harunobu/io/scene_reader.h>

#include <rapidxml/rapidxml.hpp>

namespace harunobu {

class MitsubaReader: public SceneReader {
public:
    virtual sptr<Scene> load(std::string scene_file) override;

private:
    sptr<Camera> load_camera(rapidxml::xml_node<> *node);
};

} // namespace harunobu