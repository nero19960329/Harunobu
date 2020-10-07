#include <harunobu/core/camera.h>

namespace harunobu {

Camera::Camera(const vec3 &pos_, const vec3 &dir_, const vec3 &up_, real fov_,
               int height_, int width_)
    : pos(pos_), dir(dir_), up(up_), fov(fov_), height(height_), width(width_) {
}

void Camera::log_current_status() const {
    HARUNOBU_INFO("Camera");
    HARUNOBU_INFO("--- pos = {}", glm::to_string(pos));
    HARUNOBU_INFO("--- dir = {}", glm::to_string(dir));
    HARUNOBU_INFO("--- up = {}", glm::to_string(up));
    HARUNOBU_INFO("--- fov = {}", fov);
    HARUNOBU_INFO("--- h, w = {}, {}", height, width);
}

} // namespace harunobu