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
set(OpenCV_DIR ${PROJECT_SOURCE_DIR}/include/thirdparty/opencv/lib/cmake/opencv4)
find_package(OpenCV REQUIRED)

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

file(GLOB HARUNOBU_SRC
  "src/core/*.cpp"
  "src/geometry/*.cpp"
  "src/integrator/*.cpp"
  "src/io/*.cpp"
  "src/material/*.cpp"
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
    ${OpenCV_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include)

add_executable(harunobu_bin
  src/main.cpp
)
target_include_directories(harunobu_bin
  PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(harunobu_bin
  PRIVATE
    harunobu
    spdlog::spdlog
    ${OpenCV_LIBS}
)
set_target_properties(harunobu_bin
  PROPERTIES OUTPUT_NAME harunobu)