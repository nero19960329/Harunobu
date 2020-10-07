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

file(GLOB HARUNOBU_SRC
  "src/core/*.cpp"
  "src/io/*.cpp"
  "src/geometry/*.cpp"
  "src/objects/*.cpp"
  "src/primitive/*.cpp"
)
add_library(harunobu SHARED
  ${HARUNOBU_SRC})
target_include_directories(harunobu
  PUBLIC
    ${GLM_INCLUDE_DIRS}
    ${RapidXML_INCLUDE_DIR}
    ${SPDLOG_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/include)

add_executable(harunobu_bin
  src/main.cpp
)
target_include_directories(harunobu_bin
  PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(harunobu_bin
  PRIVATE
    harunobu
    spdlog::spdlog
)
set_target_properties(harunobu_bin
  PROPERTIES OUTPUT_NAME harunobu)