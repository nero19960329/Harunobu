#include <harunobu/material/material_base.h>

HARUNOBU_NAMESPACE_BEGIN

void MaterialBase::log_current_status() const {
    HARUNOBU_INFO("MaterialBase");
    HARUNOBU_INFO("name = {}", name);
    HARUNOBU_INFO("rgb = {}", glm::to_string(rgb));
}

HARUNOBU_NAMESPACE_END