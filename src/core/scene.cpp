#include <harunobu/core/scene.h>

HARUNOBU_NAMESPACE_BEGIN

void Scene::render() {
    auto raw_image = integrator->integrate();
    image_pipeline->dump_image(raw_image);
}

HARUNOBU_NAMESPACE_END