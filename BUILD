cc_library(
    name = "harunobu_lib",
    includes = ["include"],
    hdrs = glob(["include/harunobu/**/*.h"]),
    srcs = glob(["src/**/*.cpp"]),
    copts = [
        "-std=c++17",
        "-D_FLOAT64",
    ],
    deps = [
        "@glm//:glm",
        "@spdlog//:spdlog",
        "@rapidxml//:rapidxml",
        "@libpng//:libpng",
        "@pngpp//:pngpp",
    ]
)

cc_binary(
    name = "harunobu",
    srcs = ["src/main.cpp"],
    copts = [
        "-std=c++17",
        "-D_FLOAT64",
    ],
    deps = [":harunobu_lib"]
)