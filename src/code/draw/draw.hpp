#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>
#include <filesystem>
#include "utils.hpp"
#include <img/img.hpp>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

enum class CaseType
{
    GRASS,
    PATH,
    IN,
    OUT,
    OCCUPIED
};

namespace Graph {
    struct WeightedGraphEdge 
    {
        int to {};
        float weight {1.0f};
        bool operator==(WeightedGraphEdge const& other) const = default;
        bool operator!=(WeightedGraphEdge const& other) const = default;
    };

    struct WeightedGraph
    {
        std::unordered_map<int, std::vector<WeightedGraphEdge>> adjacency_list {};

        void add_vertex(int const id);

        void add_directed_edge(int const from, int const to, float const weight = 1.0f);
        void add_undirected_edge(int const from, int const to, float const weight = 1.0f);

        bool operator==(WeightedGraph const& other) const = default;
        bool operator!=(WeightedGraph const& other) const = default;

        std::unordered_map<int, std::pair<float, int>> dijkstra(int const &start, int const end);
    };

    WeightedGraph build_from_adjacency_matrix(std::vector<std::vector<float>> const& adjacency_matrix);
}

struct Map
{
    size_t NUMBER_OF_PIXELS_IN_LINE{8};
    float MAP_SIZE{2.f};
    float SEMI_MAP_SIZE{MAP_SIZE / 2.0f};
    float PIXEL_SIZE{MAP_SIZE / NUMBER_OF_PIXELS_IN_LINE};

    GLuint _grass {};
    GLuint _path {};
    GLuint _in {};
    GLuint _out {};

    std::unordered_map<glm::vec3, CaseType> RGB_CaseType_map;
    std::vector<CaseType> px_pos_CaseType_vec;
    
    std::unordered_map<glm::vec3, CaseType> associate_RGB_to_CaseType(const std::vector<std::vector<std::string>> splitted_itd_file);
    std::vector<CaseType> associate_px_pos_to_CaseType();
    void draw_map(Map &map);
};

void draw_grid();

std::vector<std::vector<std::string>>split_itd_file();

std::vector<std::vector<float>> create_adjacency_matrix(const std::vector<std::vector<std::string>> splitted_itd_file);

void draw_quad_with_texture(GLuint const &texture, float &x, float &y, Map &map);
void draw_quad_with_texture_v2(GLuint const &texture);