#pragma once

#include <unordered_map>
#include <variant>

#include <harunobu/core/common.h>

HARUNOBU_NAMESPACE_BEGIN

using param_type = std::variant<int, real, std::string, mat4, vec3, bool>;

class AttrDict {
private:
    std::unordered_map<std::string, std::string> dict;

public:
    AttrDict() {}
    AttrDict(std::unordered_map<std::string, std::string> dict_) :
        dict(dict_) {}
    
    std::string get(std::string key) {
        if (dict.find(key) == dict.end()) {
            return key;
        } else {
            return dict[key];
        }
    }
}; // class AttrDict

class ParamSet {
private:
    std::unordered_map<std::string, param_type> dict;
    AttrDict attr_dict;

public:
    ParamSet() {}
    ParamSet(const AttrDict &attr_dict_) : attr_dict(attr_dict_) {}

    void add(std::string name, param_type val) {
        dict[attr_dict.get(name)] = val;
    }

    template <typename T> T get(std::string name, T default_value) {
        if (dict.find(name) == dict.end()) {
            return default_value;
        }
        return std::get<T>(dict[name]);
    }
}; // class ParamSet

HARUNOBU_NAMESPACE_END