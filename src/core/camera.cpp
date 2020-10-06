#include <harunobu/core/camera.h>

namespace harunobu {

Camera::Camera()
    : pos(0.0, 0.0, 0.0), dir(0.0, 0.0, 1.0), up(0.0, 1.0, 0.0), fov(30),
      height(1920), width(1080) {}

void Camera::log_current_status() const {
    HARUNOBU_INFO("Camera");
    HARUNOBU_INFO("--- pos = {}", glm::to_string(pos));
    HARUNOBU_INFO("--- dir = {}", glm::to_string(dir));
    HARUNOBU_INFO("--- up = {}", glm::to_string(up));
    HARUNOBU_INFO("--- fov = {}", fov);
    HARUNOBU_INFO("--- h, w = {}, {}", height, width);
}

} // namespace harunobu