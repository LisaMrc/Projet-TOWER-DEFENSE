#pragma once
#include <vector>
#include <unordered_map>
#include <utility>
#include <filesystem>
#include "utils.hpp"

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

enum class CaseType
{
    GRASS,
    PATH,
    IN,
    OUT,
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

        void print_DFS(int const start) const;
        void print_BFS(int const start) const;

        std::unordered_map<int, std::pair<float, int>> dijkstra(int const &start, int const end);
    };

    WeightedGraph build_from_adjacency_matrix(std::vector<std::vector<float>> const& adjacency_matrix);
}

void draw_grid();

std::vector<std::vector<std::string>>split_itd_file();

std::unordered_map<glm::vec3, CaseType> associate_RGB_to_CaseType(const std::vector<std::vector<std::string>> splitted_itd_file);
std::vector<CaseType> associate_px_pos_to_CaseType(const std::unordered_map<glm::vec3, CaseType> RGB_CaseType_map);
void draw_map(std::vector<CaseType> RGB_CaseType_map);

std::vector<std::vector<float>> create_adjacency_matrix(const std::vector<std::vector<std::string>> splitted_itd_file);

void draw_map();