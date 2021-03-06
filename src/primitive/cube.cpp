#include <harunobu/primitive/cube.h>

HARUNOBU_NAMESPACE_BEGIN

Cube::Cube(sptr<MaterialBase> material_) : PrimitiveBase(material_) {}

void Cube::init(ParamSet &param_set) {
    auto trans_mat = param_set.get<mat4>(
        "transform", mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));
    make_geos(trans_mat);
    PrimitiveBase::init(param_set);
}

std::pair<sptr<Tri>, sptr<Tri>>
Cube::make_rect_geos(const mat4 &trans_mat, const std::array<vec3, 4> &vertices,
                     const vec3 &normal) {
    sptr<Tri> tri1 = std::make_shared<Tri>(
        this, std::array<vec3, 3>{vertices[0], vertices[1], vertices[2]},
        std::array<vec3, 3>{normal, normal, normal});
    sptr<Tri> tri2 = std::make_shared<Tri>(
        this, std::array<vec3, 3>{vertices[2], vertices[3], vertices[0]},
        std::array<vec3, 3>{normal, normal, normal});
    tri1->do_transform(trans_mat);
    tri2->do_transform(trans_mat);
    return std::make_pair(tri1, tri2);
}

void Cube::make_geos(const mat4 &trans_mat) {
    geos.clear();

    std::vector<std::array<vec3, 4>> rects;
    std::vector<vec3> normals;
    // back
    rects.push_back(std::array<vec3, 4>{vec3(1, -1, 1), vec3(1, -1, -1),
                                        vec3(-1, -1, -1), vec3(-1, -1, 1)});
    normals.push_back(vec3(0, -1, 0));
    // front
    rects.push_back(std::array<vec3, 4>{vec3(-1, 1, 1), vec3(-1, 1, -1),
                                        vec3(1, 1, -1), vec3(1, 1, 1)});
    normals.push_back(vec3(0, 1, 0));
    // left
    rects.push_back(std::array<vec3, 4>{vec3(-1, -1, 1), vec3(-1, -1, -1),
                                        vec3(-1, 1, -1), vec3(-1, 1, 1)});
    normals.push_back(vec3(-1, 0, 0));
    // right
    rects.push_back(std::array<vec3, 4>{vec3(1, 1, 1), vec3(1, 1, -1),
                                        vec3(1, -1, -1), vec3(1, -1, 1)});
    normals.push_back(vec3(1, 0, 0));
    // ceil
    rects.push_back(std::array<vec3, 4>{vec3(-1, -1, 1), vec3(-1, 1, 1),
                                        vec3(1, 1, 1), vec3(1, -1, 1)});
    normals.push_back(vec3(0, 0, 1));
    // floor
    rects.push_back(std::array<vec3, 4>{vec3(-1, 1, -1), vec3(-1, -1, -1),
                                        vec3(1, -1, -1), vec3(1, 1, -1)});
    normals.push_back(vec3(0, 0, -1));

    for (size_t i = 0; i < rects.size(); ++i) {
        auto [tri1, tri2] = make_rect_geos(trans_mat, rects[i], normals[i]);
        tri1->idx = geos.size();
        geos.push_back(tri1);
        tri2->idx = geos.size();
        geos.push_back(tri2);
    }
}

void Cube::log_current_status() const {
    HARUNOBU_INFO("Cube");
    material->log_current_status();
    HARUNOBU_INFO("emit_radiance = {}", glm::to_string(emit_radiance));
    for (const auto &geo : geos) {
        geo->log_current_status();
    }
}

HARUNOBU_NAMESPACE_END