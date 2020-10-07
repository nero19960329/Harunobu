#include <harunobu/core/camera.h>

namespace harunobu {

Camera::Camera(const vec3 &pos_, const vec3 &dir_, const vec3 &up_, real fov_,
               int height_, int width_)
    : pos(pos_), dir(dir_), up(up_), fov(fov_), height(height_), width(width_) {
}

Ray Camera::make_ray(int i, int j) const {
    vec3 ray_dst = pos;
    ray_dst += dir * static_cast<real>(1);
    real h = 2.0 * tan(fov * 0.5);
    real pixel = h / static_cast<real>(height);
    ray_dst -=
        up * static_cast<real>((height - 1) * 0.5 - j) * pixel +
        glm::cross(dir, up) * static_cast<real>((width - 1) * 0.5 - i) * pixel;
    vec3 ray_dir = glm::normalize(ray_dst - pos);
    return Ray(pos, ray_dir);
}

void Camera::log_current_status() const {
    HARUNOBU_INFO("Camera");
    HARUNOBU_INFO("pos = {}", glm::to_string(pos));
    HARUNOBU_INFO("dir = {}", glm::to_string(dir));
    HARUNOBU_INFO("up = {}", glm::to_string(up));
    HARUNOBU_INFO("fov = {}", fov);
    HARUNOBU_INFO("h, w = {}, {}", height, width);
}

} // namespace harunobu