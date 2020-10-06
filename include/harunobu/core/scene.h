#pragma once

#include <harunobu/core/camera.h>
#include <harunobu/core/common.h>
#include <harunobu/objects/objects_base.h>

namespace harunobu {

class Scene {
public:
    sptr<Camera> camera;
    sptr<ObjectsBase> objects;
}; // class Scene

} // namespace harunobu