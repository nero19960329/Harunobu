#include <harunobu/core/utils.h>
#include <harunobu/io/mitsuba_reader.h>
#include <harunobu/material/bsdf.h>

#include <rapidxml/rapidxml_utils.hpp>

#include <cstring>
#include <sstream>
#include <vector>

namespace harunobu {

//============================== Utils ==============================

#define CHECK_ATTR(node, attr_name)                                            \
    HARUNOBU_CHECK(node->first_attribute(attr_name) != nullptr,                \
                   "'{}' node does not have attribute '{}'!", node->name(),    \
                   attr_name);

#define CHECK_ATTR_VALUE(node, attr_name, attr_value)                          \
    CHECK_ATTR(node, attr_name);                                               \
    HARUNOBU_CHECK(                                                            \
        str_equal(node->first_attribute(attr_name)->value(), attr_value),      \
        "'{}' is not supported for attribute '{}' at '{}' node!",              \
        node->first_attribute(attr_name)->value(), attr_name, node->name());

#define IGNORE_ATTR(node, attr_name)                                           \
    HARUNOBU_WARN("Attribute '{}' is not supported at '{}' node, ignored.",    \
                  attr_name, node->name());

#define IGNORE_SUBNODE(node, subnode_name)                                     \
    HARUNOBU_WARN("Subnode '{}' is not supported under '{}' node, ignored.",   \
                  subnode_name, node->name());

#define CHECK_ANY_SUBNODE(node, ...)                                           \
    {                                                                          \
        std::vector<const char *> subnode_names(__VA_ARGS__);                  \
        HARUNOBU_CHECK(has_any_subnode(node, subnode_names),                   \
                       "'{}' node does not have any of subnode '{}'!",         \
                       node->name(), concat_str(subnode_names));               \
    }

inline bool str_equal(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

inline bool has_any_subnode(rapidxml::xml_node<> *node,
                            const std::vector<const char *> &subnode_names) {
    for (const auto &subnode_name : subnode_names) {
        if (node->first_node(subnode_name) != nullptr) {
            return true;
        }
    }
    return false;
}

inline std::string concat_str(const std::vector<const char *> &strs) {
    std::string subnode_names_str = "[ ";
    for (const auto &s : strs) {
        subnode_names_str += s;
        subnode_names_str += " ";
    }
    subnode_names_str += "]";
    return subnode_names_str;
}

inline mat4 load_mat4(rapidxml::xml_attribute<> *mat_attr) {
    std::string mat_str = mat_attr->value();
    mat4 mat;
    std::istringstream iss(mat_str);
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) {
            iss >> mat[i][j];
        }
    return mat;
}

inline vec3 load_vec3(rapidxml::xml_attribute<> *vec_attr) {
    // xxx xxx xxx
    std::string vec_str = vec_attr->value();
    vec3 vec;
    std::istringstream iss(vec_str);
    for (int i = 0; i < 3; ++i) {
        iss >> vec[i];
    }
    return vec;
}

inline vec3 load_rgb(rapidxml::xml_attribute<> *rgb_attr) {
    // xxx, xxx, xxx
    std::string rgb_str = rgb_attr->value();
    vec3 vec;
    std::istringstream iss(rgb_str);
    for (int i = 0; i < 3; ++i) {
        iss >> vec[i];
        char tmp;
        iss >> tmp;
    }
    return vec;
}

//============================ Class functions ============================

sptr<Scene> MitsubaReader::load(std::string scene_file) {
    rapidxml::file<> scene_xml(scene_file.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(scene_xml.data());
    auto scene_node = doc.first_node("scene");
    HARUNOBU_CHECK(scene_node != nullptr, "There is no scene node!");
    CHECK_ANY_SUBNODE(scene_node, {"sensor"});
    auto camera = load_camera(scene_node->first_node("sensor"));
    auto materials = load_materials(scene_node);

    return std::make_shared<Scene>();
}

sptr<Camera> MitsubaReader::load_camera(rapidxml::xml_node<> *camera_node) {
    HARUNOBU_DEBUG("Loading camera ...");

    HARUNOBU_CHECK(camera_node != nullptr, "There is no camera node!");
    CHECK_ATTR_VALUE(camera_node, "type", "perspective");

    sptr<Camera> camera = std::make_shared<Camera>();

    // Load floats
    for (auto node = camera_node->first_node("float"); node != nullptr;
         node = node->next_sibling("float")) {
        auto node_name = node->first_attribute("name")->value();
        HARUNOBU_DEBUG("Traversing node {}-{}", node->name(), node_name);
        if (str_equal(node_name, "fov")) {
            camera->fov = atof(node->first_attribute("value")->value());
        } else {
            IGNORE_ATTR(camera_node, node_name);
        }
    }

    // Load transform
    auto trans_node = camera_node->first_node("transform");
    CHECK_ATTR_VALUE(trans_node, "name", "toWorld");
    CHECK_ANY_SUBNODE(trans_node, {"matrix", "lookat"});
    auto trans_mat_node = trans_node->first_node("matrix");
    auto trans_lookat_node = trans_node->first_node("lookat");
    if (trans_mat_node != nullptr) {
        HARUNOBU_DEBUG("Traversing node {}-{}", trans_node->name(),
                       trans_mat_node->name());
        CHECK_ATTR(trans_mat_node, "value");
        auto trans_mat = load_mat4(trans_mat_node->first_attribute("value"));
        camera->pos = homo2carte(vec4(0.0, 0.0, 0.0, 1.0) * trans_mat);
        auto target = homo2carte(vec4(0.0, 0.0, 1.0, 1.0) * trans_mat);
        camera->dir = glm::normalize(target - camera->pos);
        // direction does not need to be converted
        camera->up = vec3(vec4(0.0, 1.0, 0.0, 0.0) * trans_mat);
    } else if (trans_lookat_node != nullptr) {
        HARUNOBU_DEBUG("Traversing node {}-{}", trans_node->name(),
                       trans_lookat_node->name());
        camera->pos = load_vec3(trans_lookat_node->first_attribute("origin"));
        auto target = load_vec3(trans_lookat_node->first_attribute("target"));
        camera->dir = glm::normalize(target - camera->pos);
        camera->up = load_vec3(trans_lookat_node->first_attribute("up"));
    }

    // Load film
    auto film_node = camera_node->first_node("film");
    CHECK_ATTR_VALUE(film_node, "type", "ldrfilm");
    for (auto node = film_node->first_node("integer"); node != nullptr;
         node = node->next_sibling("integer")) {
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

std::vector<sptr<MaterialBase>>
MitsubaReader::load_materials(rapidxml::xml_node<> *scene_node) {
    IGNORE_SUBNODE(scene_node, "brdf");
    IGNORE_SUBNODE(scene_node, "bssrdf");

    std::vector<sptr<MaterialBase>> materials;
    for (auto node = scene_node->first_node("bsdf"); node != nullptr;
         node = node->next_sibling("bsdf")) {
        CHECK_ATTR_VALUE(node, "type", "twosided");
        CHECK_ATTR(node, "id");
        CHECK_ANY_SUBNODE(node, {"bsdf"});
        auto id = node->first_attribute("id")->value();

        auto bsdf_node = node->first_node("bsdf");
        CHECK_ATTR_VALUE(bsdf_node, "type", "diffuse");
        CHECK_ANY_SUBNODE(bsdf_node, {"rgb"});

        auto rgb_node = bsdf_node->first_node("rgb");
        sptr<MaterialBase> material = std::make_shared<BSDF>();
        if (rgb_node != nullptr) {
            CHECK_ATTR_VALUE(rgb_node, "name", "reflectance");
            material->rgb = load_rgb(rgb_node->first_attribute("value"));
        }
        materials.push_back(material);
    }

    return materials;
}

} // namespace harunobu