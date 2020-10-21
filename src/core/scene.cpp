#include <harunobu/core/scene.h>

HARUNOBU_NAMESPACE_BEGIN

void Scene::render(std::string output_path) {
    auto raw_image = integrator->integrate();
    image_pipeline->dump_image(raw_image, output_path);
}

HARUNOBU_NAMESPACE_END