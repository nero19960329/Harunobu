#pragma once

#include <string>

#include <harunobu/core/common.h>
#include <harunobu/core/scene.h>

namespace harunobu {

class SceneReader {
public:
    virtual sptr<Scene> load(std::string scene_file) = 0;
};

} // namespace harunobu