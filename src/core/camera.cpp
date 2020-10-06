#include <harunobu/core/camera.h>

namespace harunobu {

void Camera::log_current_status() const {
    HARUNOBU_INFO("Camera");
    HARUNOBU_INFO("--- pos = {}", glm::to_string(pos));
    HARUNOBU_INFO("--- dir = {}", glm::to_string(dir));
    HARUNOBU_INFO("--- up = {}", glm::to_string(up));
    HARUNOBU_INFO("--- fov = {}", fov);
    HARUNOBU_INFO("--- h, w = {}, {}", height, width);
}

} // namespace harunobu