#pragma once

#include <harunobu/core/common.h>
#include <harunobu/core/film.h>
#include <harunobu/core/image.h>
#include <harunobu/core/param_set.h>
#include <harunobu/sampler/sampler_base.h>

HARUNOBU_NAMESPACE_BEGIN

class Scene;

class IntegratorBase {
public:
    sptr<Scene> scene;
    sptr<SamplerBase> sampler;
    sptr<Film> film;

public:
    IntegratorBase(sptr<Scene> scene_) : scene(scene_) {}

    virtual sptr<Image<real>> integrate() = 0;

    static sptr<IntegratorBase> factory(std::string name, sptr<Scene> scene,
                                        ParamSet &param_set);
}; // class IntegratorBase

HARUNOBU_NAMESPACE_END