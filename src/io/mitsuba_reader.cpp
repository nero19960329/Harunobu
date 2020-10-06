#include <harunobu/io/mitsuba_reader.h>

#include <rapidxml/rapidxml_utils.hpp>

#include <cstring>

namespace harunobu {

bool str_equal(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}

#define CHECK_ATTR(node, attr_name, attr_value) \
    HARUNOBU_CHECK( \
        node->first_attribute(attr_name) != nullptr, \
        "{} node does not have attribute '{}'!", node->name(), attr_name); \
    HARUNOBU_CHECK( \
        str_equal(node->first_attribute(attr_name)->value(), attr_value), \
        "'{}' is not supported for attribute '{}' at '{}' node!", \
            node->first_attribute(attr_name)->value(), \
            attr_name, node->name());

#define IGNORE_ATTR(node, attr_name) \
    HARUNOBU_WARN("Attribute '{}' is not supported at '{}' node, ignored.", attr_name, node->name());

sptr<Scene> MitsubaReader::load(std::string scene_file) {
    rapidxml::file<> scene_xml(scene_file.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(scene_xml.data());
    auto scene_node = doc.first_node("scene");
    HARUNOBU_CHECK(
        scene_node != nullptr,
        "There is no scene node!");
    auto camera = load_camera(scene_node->first_node("sensor"));

    return std::make_shared<Scene>();
}

sptr<Camera> MitsubaReader::load_camera(rapidxml::xml_node<> *camera_node) {
    HARUNOBU_DEBUG("Loading camera ...");

    HARUNOBU_CHECK(
        camera_node != nullptr,
        "There is no camera node!");
    CHECK_ATTR(camera_node, "type", "perspective");

    sptr<Camera> camera = std::make_shared<Camera>();

    // Load floats
    for (auto node = camera_node->first_node("float"); node != nullptr; node = node->next_sibling("float")) {
        auto node_name = node->first_attribute("name")->value();
        HARUNOBU_DEBUG("Traversing node {}-{}", node->name(), node_name);
        if (str_equal(node_name, "fov")) {
            camera->fov = atof(node->first_attribute("value")->value());
        } else {
            IGNORE_ATTR(camera_node, node_name);
        }
    }

    // Load transform
    /*auto trans_node = camera_node->first_node("transform");
    HARUNOBU_CHECK(
        trans_node->first_attribute("name") == nullptr ||
        str_equal(trans_node->first_attribute("name")->value(), "toWorld"),
        "Transform in camera node does not have attribute 'name' or its 'name' is not supported!");
    HARUNOBU_CHECK(
        trans_node->first_node("matrix") == nullptr ||
    )*/

    // Load film
    auto film_node = camera_node->first_node("film");
    CHECK_ATTR(film_node, "type", "ldrfilm");
    for (auto node = film_node->first_node("integer"); node != nullptr; node = node->next_sibling("integer")) {
        auto node_name = node->first_attribute("name")->value();
        HARUNOBU_DEBUG("Traversing node {}-{}", node->name(), node_name);
        if (str_equal(node_name, "width")) {
            camera->width = atoi(node->first_attribute("value")->value());
        } else if (str_equal(node_name, "height")) {
            camera->height = atoi(node->first_attribute("value")->value());
        } else {
            IGNORE_ATTR(camera_node, node_name);
        }
    }
    
    camera->log_current_status();
    return camera;
}

} // namespace harunobu