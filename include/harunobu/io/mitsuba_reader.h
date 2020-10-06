#pragma once

#include <harunobu/core/camera.h>
#include <harunobu/io/scene_reader.h>
#include <harunobu/material/material_base.h>

#include <rapidxml/rapidxml.hpp>

#include <vector>

namespace harunobu {

class MitsubaReader: public SceneReader {
public:
    virtual sptr<Scene> load(std::string scene_file) override;

private:
    sptr<Camera> load_camera(rapidxml::xml_node<> *camera_node);
    std::vector<sptr<MaterialBase>> load_materials(rapidxml::xml_node<> *scene_node);
};

} // namespace harunobu