cc_library(
    name = "harunobu_lib",
    includes = ["include"],
    hdrs = glob(["include/harunobu/**/*.h"]),
    srcs = glob(["src/**/*.cpp"]),
    copts = [
        "-std=c++17",
        "-D_FLOAT64",
        "-fopenmp",
    ],
    linkopts = ["-lgomp"],
    deps = [
        "@glm//:glm",
        "@spdlog//:spdlog",
        "@rapidxml//:rapidxml",
    ]
)

cc_binary(
    name = "harunobu",
    srcs = ["src/main.cpp"],
    copts = [
        "-std=c++17",
        "-D_FLOAT64",
        "-fopenmp",
    ],
    linkopts = ["-lgomp"],
    deps = [":harunobu_lib"]
)