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
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            iss >> mat[i][j];
        }
    }
    HARUNOBU_DEBUG("Load mat4 = {}", glm::to_string(mat));
    return mat;
}

inline vec3 load_vec3(rapidxml::xml_attribute<> *vec_attr) {
    // xxx, xxx, xxx
    std::string vec_str = vec_attr->value();
    vec3 vec;
    std::istringstream iss(vec_str);
    for (int i = 0; i < 3; ++i) {
        iss >> vec[i];
        char tmp;
        iss >> tmp;
    }
    HARUNOBU_DEBUG("Load vec3 = {}", glm::to_string(vec));
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
    materials = load_materials(scene_node);
    auto objects = load_objects(scene_node);

    return std::make_shared<Scene>();
}

sptr<Camera> MitsubaReader::load_camera(rapidxml::xml_node<> *camera_node) {
    HARUNOBU_DEBUG("Loading camera ...");

    HARUNOBU_CHECK(camera_node != nullptr, "There is no camera node!");
    CHECK_ATTR_VALUE(camera_node, "type", "perspective");

    vec3 pos, dir, up;
    real fov;
    int height, width;

    // Load floats
    for (auto node = camera_node->first_node("float"); node != nullptr;
         node = node->next_sibling("float")) {
        auto node_name = node->first_attribute("name")->value();
        HARUNOBU_DEBUG("Traversing node {}-{}", node->name(), node_name);
        if (str_equal(node_name, "fov")) {
            fov = atof(node->first_attribute("value")->value());
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
        pos = homo2carte(vec4(0.0, 0.0, 0.0, 1.0) * trans_mat);
        auto target = homo2carte(vec4(0.0, 0.0, 1.0, 1.0) * trans_mat);
        dir = glm::normalize(target - pos);
        // direction does not need to be converted
        up = vec3(vec4(0.0, 1.0, 0.0, 0.0) * trans_mat);
    } else if (trans_lookat_node != nullptr) {
        HARUNOBU_DEBUG("Traversing node {}-{}", trans_node->name(),
                       trans_lookat_node->name());
        pos = load_vec3(trans_lookat_node->first_attribute("origin"));
        auto target = load_vec3(trans_lookat_node->first_attribute("target"));
        dir = glm::normalize(target - pos);
        up = load_vec3(trans_lookat_node->first_attribute("up"));
    }

    // Load film
    auto film_node = camera_node->first_node("film");
    CHECK_ATTR_VALUE(film_node, "type", "ldrfilm");
    for (auto node = film_node->first_node("integer"); node != nullptr;
         node = node->next_sibling("integer")) {
        auto node_name = node->first_attribute("name")->value();
        HARUNOBU_DEBUG("Traversing node {}-{}", node->name(), node_name);
        if (str_equal(node_name, "width")) {
            width = atoi(node->first_attribute("value")->value());
        } else if (str_equal(node_name, "height")) {
            height = atoi(node->first_attribute("value")->value());
        } else {
            IGNORE_ATTR(camera_node, node_name);
        }
    }

    sptr<Camera> camera =
        std::make_shared<Camera>(pos, dir, up, fov, height, width);
    camera->log_current_status();
    return camera;
}

std::unordered_map<std::string, sptr<MaterialBase>>
MitsubaReader::load_materials(rapidxml::xml_node<> *scene_node) {
    auto ignore_node_names = {"brdf", "bssrdf"};
    for (const auto &name : ignore_node_names) {
        if (scene_node->first_node(name) != nullptr) {
            IGNORE_SUBNODE(scene_node, name);
        }
    }

    std::unordered_map<std::string, sptr<MaterialBase>> materials;
    for (auto node = scene_node->first_node("bsdf"); node != nullptr;
         node = node->next_sibling("bsdf")) {
        CHECK_ATTR_VALUE(node, "type", "twosided");
        CHECK_ATTR(node, "id");
        CHECK_ANY_SUBNODE(node, {"bsdf"});
        std::string id = node->first_attribute("id")->value();

        auto bsdf_node = node->first_node("bsdf");
        CHECK_ATTR_VALUE(bsdf_node, "type", "diffuse");
        CHECK_ANY_SUBNODE(bsdf_node, {"rgb"});

        auto rgb_node = bsdf_node->first_node("rgb");
        sptr<MaterialBase> material = std::make_shared<BSDF>();
        if (rgb_node != nullptr) {
            CHECK_ATTR_VALUE(rgb_node, "name", "reflectance");
            material->name = id;
            material->rgb = load_vec3(rgb_node->first_attribute("value"));
        }
        materials[id] = material;
    }

    return materials;
}

sptr<ObjectsBase>
MitsubaReader::load_objects(rapidxml::xml_node<> *scene_node) {
    sptr<ObjectsBase> objects = std::make_shared<ObjectsBase>();

    for (auto node = scene_node->first_node("shape"); node != nullptr;
         node = node->next_sibling("shape")) {
        CHECK_ANY_SUBNODE(node, {"transform"});
        CHECK_ANY_SUBNODE(node, {"ref"});
        CHECK_ATTR(node, "type");

        auto trans_node = node->first_node("transform");
        CHECK_ATTR_VALUE(trans_node, "name", "toWorld");
        CHECK_ANY_SUBNODE(trans_node, {"matrix"});

        auto trans_mat_node = trans_node->first_node("matrix");
        CHECK_ATTR(trans_mat_node, "value");
        auto trans_mat = load_mat4(trans_mat_node->first_attribute("value"));

        auto ref_node = node->first_node("ref");
        CHECK_ATTR(ref_node, "id");
        std::string ref_mate_id = ref_node->first_attribute("id")->value();
        HARUNOBU_CHECK(materials.find(ref_mate_id) != materials.end(),
                       "Material '{}' is not mentioned before!", ref_mate_id);
        auto material = materials[ref_mate_id];

        std::string prim_name = node->first_attribute("type")->value();
        auto prim = PrimitiveBase::factory(prim_name, material, trans_mat);
        prim->log_current_status();
        objects->add_primitive(prim);
    }

    HARUNOBU_CHECK(objects->build(), "Objects not partitioned successfully!");
    return objects;
}

} // namespace harunobu