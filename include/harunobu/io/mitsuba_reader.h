#pragma once

#include <harunobu/core/camera.h>
#include <harunobu/core/film.h>
#include <harunobu/core/image_pipeline.h>
#include <harunobu/integrator/integrator_base.h>
#include <harunobu/io/scene_reader.h>
#include <harunobu/material/material_base.h>
#include <harunobu/objects/objects_base.h>
#include <harunobu/rfilter/rfilter_base.h>
#include <harunobu/sampler/sampler_base.h>

#include <rapidxml/rapidxml.hpp>

#include <unordered_map>
#include <vector>

HARUNOBU_NAMESPACE_BEGIN

class MitsubaReader : public SceneReader {
private:
    std::unordered_map<std::string, sptr<MaterialBase>> materials;

public:
    virtual sptr<Scene> load(std::string scene_file) override;

private:
    sptr<IntegratorBase> load_integrator(rapidxml::xml_node<> *integrator_node,
                                         sptr<Scene> scene);
    sptr<SamplerBase> load_sampler(rapidxml::xml_node<> *sampler_node);
    sptr<Camera> load_camera(rapidxml::xml_node<> *camera_node);
    std::unordered_map<std::string, sptr<MaterialBase>>
    load_materials(rapidxml::xml_node<> *scene_node);
    std::pair<sptr<ObjectsBase>, sptr<ObjectsBase>>
    load_objects(rapidxml::xml_node<> *scene_node);
    sptr<ImagePipeline> load_image_pipeline(rapidxml::xml_node<> *film_node);
    sptr<RFilterBase> load_rfilter(rapidxml::xml_node<> *rfilter_node);
    sptr<Film> load_film(rapidxml::xml_node<> *film_node);
};

HARUNOBU_NAMESPACE_END