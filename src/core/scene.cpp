#include <harunobu/core/scene.h>

namespace harunobu {

cv::Mat Scene::render() {
    auto raw_image = integrator->integrate();
    return raw_image;
}

} // namespace harunobu