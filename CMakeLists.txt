cmake_minimum_required(VERSION 3.5)

project (harunobu)

# Add module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# C++11 support
set (CMAKE_CXX_STANDARD 17)

# Libraries
find_package(GLM REQUIRED)
find_package(RapidXML REQUIRED)
find_package(spdlog REQUIRED)

file(GLOB HARUNOBU_CORE_SRC
  "src/core/*.cpp"
)
add_library(harunobu_core SHARED
  ${HARUNOBU_CORE_SRC}
)
target_include_directories(harunobu_core
  PUBLIC
    ${GLM_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
)

file(GLOB HARUNOBU_IO_SRC
  "src/io/*.cpp"
)
add_library(harunobu_io SHARED
  ${HARUNOBU_IO_SRC}
)
target_include_directories(harunobu_io
  PUBLIC
    ${GLM_INCLUDE_DIRS}
    ${RapidXML_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(harunobu_io
  PUBLIC
    harunobu_core)

add_executable(harunobu
  src/main.cpp
)
target_include_directories(harunobu
  PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(harunobu
  PRIVATE
    harunobu_core
    harunobu_io
    spdlog::spdlog
)