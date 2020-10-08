#pragma once

#include <iostream>

// logger
#ifdef NDEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#endif
#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <cmath>
#include <cstdlib>
#include <memory>

namespace harunobu {

// math constants
constexpr double pi() { return std::atan(1) * 4; }

// macros
#define HARUNOBU_NAMESPACE_BEGIN namespace harunobu {
#define HARUNOBU_NAMESPACE_END }

// logger
inline void initialize_logger() {
    // https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
    spdlog::set_pattern("[src %s] [func %!] [line %#] [%^%l%$] %v");
#ifdef NDEBUG
    spdlog::set_level(spdlog::level::info);
#else
    spdlog::set_level(spdlog::level::debug);
#endif
}
#define HARUNOBU_TRACE SPDLOG_TRACE
#define HARUNOBU_DEBUG SPDLOG_DEBUG
#define HARUNOBU_INFO SPDLOG_INFO
#define HARUNOBU_WARN SPDLOG_WARN
#define HARUNOBU_ERROR SPDLOG_ERROR
#define HARUNOBU_CRITICAL SPDLOG_CRITICAL

// global float precision
#ifdef _FLOAT64
using real = double;
using vec2 = glm::dvec2;
using vec3 = glm::dvec3;
using vec4 = glm::dvec4;
using mat2 = glm::dmat2;
using mat3 = glm::dmat3;
using mat4 = glm::dmat4;
#else
using real = float;
using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;
using mat2 = glm::mat2;
using mat3 = glm::mat3;
using mat4 = glm::mat4;
#endif

// abbrs
template <typename T> using sptr = std::shared_ptr<T>;

// assertion
#ifdef NDEBUG
#define HARUNOBU_ASSERT(x, ...)
#else
#define HARUNOBU_ASSERT(x, ...)                                                \
    {                                                                          \
        if (!(x)) {                                                            \
            HARUNOBU_ERROR(__VA_ARGS__);                                       \
            HARUNOBU_ERROR("Assertion failed: {}, abort.", #x);                \
            exit(1);                                                           \
        }                                                                      \
    }
#endif
#define HARUNOBU_CHECK(x, ...)                                                 \
    {                                                                          \
        if (!(x)) {                                                            \
            HARUNOBU_CRITICAL(__VA_ARGS__);                                    \
            HARUNOBU_CRITICAL("Check failed: {}, abort.", #x);                 \
            exit(1);                                                           \
        }                                                                      \
    }

// initialize all envs
inline void initialize_env() { initialize_logger(); }

} // namespace harunobu
