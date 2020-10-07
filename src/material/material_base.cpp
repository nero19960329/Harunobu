#include <harunobu/material/material_base.h>

namespace harunobu {

void MaterialBase::log_current_status() const {
    HARUNOBU_INFO("MaterialBase");
    HARUNOBU_INFO("name = {}", name);
    HARUNOBU_INFO("rgb = {}", glm::to_string(rgb));
}

} // namespace harunobu