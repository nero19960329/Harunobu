#pragma once

#include <unordered_map>
#include <variant>

#include <harunobu/core/common.h>

HARUNOBU_NAMESPACE_BEGIN

using param_type = std::variant<int, real, std::string, mat4, vec3, bool>;

class ParamSet {
private:
    std::unordered_map<std::string, param_type> dict;

public:
    ParamSet() {}

    void add(std::string name, param_type val) { dict[name] = val; }

    template <typename T> T get(std::string name, T default_value) {
        if (dict.find(name) == dict.end()) {
            return default_value;
        }
        return std::get<T>(dict[name]);
    }
}; // class ParamSet

HARUNOBU_NAMESPACE_END