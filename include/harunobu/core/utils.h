#pragma once

#include <harunobu/core/common.h>

HARUNOBU_NAMESPACE_BEGIN

inline vec3 reflect(const vec3 &wo, const vec3 &n) {
    return -wo + 2 * glm::dot(wo, n) * n;
}

inline vec3 homo2carte(const vec4 &v) {
    HARUNOBU_ASSERT(v[3] != 0.0, "Homogeneous element equals to 0!");
    return vec3(v / v[3]);
}

inline std::vector<std::string> split(const std::string &s,
                                      const std::string &delimiters) {
    std::vector<std::string> tokens;
    std::string::size_type last_pos = s.find_first_not_of(delimiters, 0);
    std::string::size_type pos = s.find_first_of(delimiters, last_pos);
    while (std::string::npos != pos || std::string::npos != last_pos) {
        tokens.push_back(s.substr(last_pos, pos - last_pos));
        last_pos = s.find_first_not_of(delimiters, pos);
        pos = s.find_first_of(delimiters, last_pos);
    }
    return tokens;
}

HARUNOBU_NAMESPACE_END