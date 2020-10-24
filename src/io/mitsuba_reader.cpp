#include <harunobu/core/param_set.h>
#include <harunobu/core/utils.h>
#include <harunobu/io/mitsuba_reader.h>
#include <harunobu/sampler/random_sampler.h>
#include <harunobu/sampler/stratified_sampler.h>

#include <rapidxml_utils.hpp>

#include <cstring>
#include <sstream>
#include <vector>

HARUNOBU_NAMESPACE_BEGIN

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

template <typename T>
void load_param(rapidxml::xml_node<> *node, ParamSet &param_set,
                std::vector<std::string> node_names) {
    for (const auto &name : node_names) {
        for (auto subnode = node->first_node(name.c_str()); subnode != nullptr;
             subnode = subnode->next_sibling(name.c_str())) {
            CHECK_ATTR(subnode, "name");
            auto attr_name = subnode->first_attribute("name")->value();
            if (std::is_same<T, int>::value) {
                CHECK_ATTR(subnode, "value");
                param_set.add(attr_name,
                              atoi(subnode->first_attribute("value")->value()));
            } else if (std::is_same<T, real>::value) {
                CHECK_ATTR(subnode, "value");
                param_set.add(attr_name,
                              atof(subnode->first_attribute("value")->value()));
            } else if (std::is_same<T, vec3>::value) {
                if (subnode->first_attribute("value") != nullptr) {
                    param_set.add(attr_name,
                                  load_vec3(subnode->first_attribute("value")));
                } else if (subnode->first_attribute("x") != nullptr) {
                    CHECK_ATTR(subnode, "y");
                    CHECK_ATTR(subnode, "z");
                    param_set.add(
                        attr_name,
                        vec3(atof(subnode->first_attribute("x")->value()),
                             atof(subnode->first_attribute("y")->value()),
                             atof(subnode->first_attribute("z")->value())));
                } else {
                    HARUNOBU_CHECK(false, "Unsupported node type {}.",
                                   attr_name);
                }
            } else if (std::is_same<T, mat4>::value) {
                CHECK_ATTR(subnode, "value");
                param_set.add(attr_name,
                              load_mat4(subnode->first_attribute("value")));
            }
        }
    }
}

//============================ Class functions ============================

sptr<Scene> MitsubaReader::load(std::string scene_file) {
    HARUNOBU_INFO("Loading scene {} ...", scene_file);

    auto output_name = split(split(scene_file, "/").back(), ".").front();
    HARUNOBU_DEBUG("Output name is {} ...", output_name);

    rapidxml::file<> scene_xml(scene_file.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(scene_xml.data());

    sptr<Scene> scene = std::make_shared<Scene>();

    auto scene_node = doc.first_node("scene");
    HARUNOBU_CHECK(scene_node != nullptr, "There is no scene node!");
    CHECK_ANY_SUBNODE(scene_node, {"sensor"});
    CHECK_ANY_SUBNODE(scene_node, {"integrator"});
    auto integrator =
        load_integrator(scene_node->first_node("integrator"), scene);
    auto camera_node = scene_node->first_node("sensor");
    CHECK_ANY_SUBNODE(camera_node, {"sampler"});
    auto sampler = load_sampler(camera_node->first_node("sampler"));
    auto camera = load_camera(camera_node);
    materials = load_materials(scene_node);
    auto [objects, lights] = load_objects(scene_node);
    CHECK_ANY_SUBNODE(camera_node, {"film"});
    auto image_pipeline = load_image_pipeline(camera_node->first_node("film"));
    image_pipeline->file_name = output_name;
    auto film = load_film(camera_node->first_node("film"));

    scene->integrator = integrator;
    scene->integrator->sampler = sampler;
    scene->integrator->film = film;
    scene->camera = camera;
    scene->objects = objects;
    scene->lights = lights;
    scene->image_pipeline = image_pipeline;

    return scene;
}

sptr<IntegratorBase>
MitsubaReader::load_integrator(rapidxml::xml_node<> *integrator_node,
                               sptr<Scene> scene) {
    HARUNOBU_DEBUG("Loading integrator ...");
    CHECK_ATTR(integrator_node, "type");

    AttrDict type_map({{"direct", "direct"}, {"path", "path_tracer"}});
    AttrDict name_map({{"maxDepth", "max_depth"}, {"emitterSamples", "light_sample_num"}, {"bsdfSamples", "bsdf_sample_num"}});
    ParamSet param_set(name_map);
    load_param<int>(integrator_node, param_set, {"integer"});

    std::string mitsuba_integrator_type =
        integrator_node->first_attribute("type")->value();
    return IntegratorBase::factory(type_map.get(mitsuba_integrator_type), scene,
                                   param_set);
}

sptr<SamplerBase>
MitsubaReader::load_sampler(rapidxml::xml_node<> *sampler_node) {
    HARUNOBU_DEBUG("Loading sampler ...");
    CHECK_ATTR(sampler_node, "type");

    AttrDict type_map({{"independent", "random"}, {"stratified", "stratified"}});
    AttrDict name_map({
        {"sampleCount", "sample_count"},
        {"dimension", "dim_valid"}});

    ParamSet param_set(name_map);
    load_param<int>(sampler_node, param_set, {"integer"});

    std::string mitsuba_sampler_type = 
        sampler_node->first_attribute("type")->value();
    return SamplerBase::factory(type_map.get(mitsuba_sampler_type), param_set);
}

sptr<Camera> MitsubaReader::load_camera(rapidxml::xml_node<> *camera_node) {
    HARUNOBU_DEBUG("Loading camera ...");
    CHECK_ATTR_VALUE(camera_node, "type", "perspective");

    vec3 pos, dir, up;
    real fov;

    // Load floats
    for (auto node = camera_node->first_node("float"); node != nullptr;
         node = node->next_sibling("float")) {
        auto node_name = node->first_attribute("name")->value();
        HARUNOBU_DEBUG("Traversing node {}-{}", node->name(), node_name);
        if (str_equal(node_name, "fov")) {
            fov = atof(node->first_attribute("value")->value()) * pi() / 180.0;
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
        pos = homo2carte(vec4(0, 0, 0, 1) * trans_mat);
        auto target = homo2carte(vec4(0, 0, 1, 1) * trans_mat);
        dir = glm::normalize(target - pos);
        // direction does not need to be converted
        up = vec3(vec4(0, 1, 0, 0) * trans_mat);
    } else if (trans_lookat_node != nullptr) {
        HARUNOBU_DEBUG("Traversing node {}-{}", trans_node->name(),
                       trans_lookat_node->name());
        pos = load_vec3(trans_lookat_node->first_attribute("origin"));
        auto target = load_vec3(trans_lookat_node->first_attribute("target"));
        dir = glm::normalize(target - pos);
        up = load_vec3(trans_lookat_node->first_attribute("up"));
    }

    sptr<Camera> camera = std::make_shared<Camera>(pos, dir, up, fov);
    camera->log_current_status();
    return camera;
}

std::unordered_map<std::string, sptr<MaterialBase>>
MitsubaReader::load_materials(rapidxml::xml_node<> *scene_node) {
    HARUNOBU_DEBUG("Loading materials ...");

    auto ignore_node_names = {"brdf", "bssrdf"};
    for (const auto &name : ignore_node_names) {
        if (scene_node->first_node(name) != nullptr) {
            IGNORE_SUBNODE(scene_node, name);
        }
    }

    std::unordered_map<std::string, sptr<MaterialBase>> materials;
    for (auto node = scene_node->first_node("bsdf"); node != nullptr;
         node = node->next_sibling("bsdf")) {
        CHECK_ATTR(node, "type");
        CHECK_ATTR(node, "id");
        std::string id = node->first_attribute("id")->value();
        auto bsdf_node = node;
        bool is_two_sided = false;
        if (str_equal(node->first_attribute("type")->value(), "twosided")) {
            CHECK_ANY_SUBNODE(node, {"bsdf"});
            bsdf_node = node->first_node("bsdf");
            is_two_sided = true;
        }

        CHECK_ATTR(bsdf_node, "type");
        ParamSet param_set;
        load_param<vec3>(bsdf_node, param_set, {"rgb"});
        load_param<real>(bsdf_node, param_set, {"float"});
        load_param<std::string>(bsdf_node, param_set, {"string"});
        param_set.add("is_two_sided", is_two_sided);
        param_set.add("id", id);

        std::string material_name = bsdf_node->first_attribute("type")->value();
        sptr<MaterialBase> material =
            MaterialBase::factory(material_name, param_set);
        materials[id] = material;
    }

    return materials;
}

void load_object_param(rapidxml::xml_node<> *shape_node, ParamSet &param_set) {
    auto trans_node = shape_node->first_node("transform");
    if (trans_node != nullptr) {
        CHECK_ATTR_VALUE(trans_node, "name", "toWorld");
        auto trans_mat_node = trans_node->first_node("matrix");
        CHECK_ATTR(trans_mat_node, "value");
        auto trans_mat = load_mat4(trans_mat_node->first_attribute("value"));
        param_set.add("transform", trans_mat);
    }

    load_param<real>(shape_node, param_set, {"float"});
    load_param<vec3>(shape_node, param_set, {"point"});
}

std::pair<sptr<ObjectsBase>, sptr<ObjectsBase>>
MitsubaReader::load_objects(rapidxml::xml_node<> *scene_node) {
    HARUNOBU_DEBUG("Loading objects & lights ...");

    sptr<ObjectsBase> objects = std::make_shared<ObjectsBase>();
    sptr<ObjectsBase> lights = std::make_shared<ObjectsBase>();

    for (auto node = scene_node->first_node("shape"); node != nullptr;
         node = node->next_sibling("shape")) {
        CHECK_ANY_SUBNODE(node, {"ref"});
        CHECK_ATTR(node, "type");

        auto ref_node = node->first_node("ref");
        CHECK_ATTR(ref_node, "id");
        std::string ref_mate_id = ref_node->first_attribute("id")->value();
        HARUNOBU_CHECK(materials.find(ref_mate_id) != materials.end(),
                       "Material '{}' is not mentioned before!", ref_mate_id);
        auto material = materials[ref_mate_id];

        ParamSet param_set;
        load_object_param(node, param_set);

        auto emitter_node = node->first_node("emitter");
        vec3 emit_radiance(0, 0, 0);
        if (emitter_node != nullptr) {
            CHECK_ANY_SUBNODE(emitter_node, {"rgb"});
            auto emitter_rgb_node = emitter_node->first_node("rgb");
            CHECK_ATTR_VALUE(emitter_rgb_node, "name", "radiance");
            CHECK_ATTR(emitter_rgb_node, "value");
            emit_radiance =
                load_vec3(emitter_rgb_node->first_attribute("value"));
        }

        std::string prim_name = node->first_attribute("type")->value();
        auto prim = PrimitiveBase::factory(prim_name, material, param_set);
        prim->emit_radiance = emit_radiance;
        prim->log_current_status();

        objects->add_primitive(prim);
        if (emitter_node != nullptr) {
            lights->add_primitive(prim);
        }
    }

    HARUNOBU_CHECK(objects->build(), "Objects not partitioned successfully!");
    HARUNOBU_CHECK(lights->build(), "Lights not partitioned successfully!");
    return std::make_pair(objects, lights);
}

sptr<ImagePipeline>
MitsubaReader::load_image_pipeline(rapidxml::xml_node<> *film_node) {
    HARUNOBU_DEBUG("Loading image_pipeline ...");

    std::string file_format = "png";
    std::string pixel_format = "rgb";
    float gamma = 2.2;

    for (auto node = film_node->first_node("string"); node != nullptr;
         node = node->next_sibling("string")) {
        CHECK_ATTR(node, "name");
        auto node_name = node->first_attribute("name")->value();
        if (str_equal(node_name, "fileFormat")) {
            file_format = node->first_attribute("value")->value();
        } else if (str_equal(node_name, "pixelFormat")) {
            pixel_format = node->first_attribute("value")->value();
        } else {
            IGNORE_SUBNODE(node, node_name);
        }
    }
    HARUNOBU_CHECK(pixel_format == "rgb",
                   "Only 'rgb' pixel_format is supported, which current "
                   "pixel_format is {}.",
                   pixel_format);

    for (auto node = film_node->first_node("float"); node != nullptr;
         node = node->next_sibling("float")) {
        CHECK_ATTR(node, "name");
        auto node_name = node->first_attribute("name")->value();
        if (str_equal(node_name, "gamma")) {
            gamma = atof(node->first_attribute("value")->value());
        } else {
            IGNORE_SUBNODE(node, node_name);
        }
    }

    sptr<ImagePipeline> image_pipeline = std::make_shared<ImagePipeline>();
    image_pipeline->gamma = gamma;
    image_pipeline->file_format = file_format;

    image_pipeline->log_current_status();
    return image_pipeline;
}

sptr<RFilterBase>
MitsubaReader::load_rfilter(rapidxml::xml_node<> *rfilter_node) {
    HARUNOBU_DEBUG("Loading rfilter ...");

    CHECK_ATTR(rfilter_node, "type");
    std::string rfilter_type = rfilter_node->first_attribute("type")->value();
    sptr<RFilterBase> rfilter = RFilterBase::factory(rfilter_type);
    return rfilter;
}

sptr<Film> MitsubaReader::load_film(rapidxml::xml_node<> *film_node) {
    HARUNOBU_DEBUG("Loading film ...");

    int width, height;

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
            IGNORE_ATTR(film_node, node_name);
        }
    }
    auto film = std::make_shared<Film>(width, height);
    CHECK_ANY_SUBNODE(film_node, {"rfilter"});
    film->rfilter = load_rfilter(film_node->first_node("rfilter"));

    film->log_current_status();
    return film;
}

HARUNOBU_NAMESPACE_END