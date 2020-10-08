#pragma once

#include <string>

#include <harunobu/core/common.h>
#include <harunobu/core/scene.h>

HARUNOBU_NAMESPACE_BEGIN

class SceneReader {
public:
    virtual sptr<Scene> load(std::string scene_file) = 0;
};

HARUNOBU_NAMESPACE_END