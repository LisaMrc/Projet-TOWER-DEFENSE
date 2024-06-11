#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include <GLHelpers.hpp>

// #include "../draw/draw.hpp"

enum class ProjectileKind
{
    Arrow,
    Fireball,
    lightning_arrow,
};

struct projectile
{
    int damages {};
    float speed {};
    float x {}; // position x
    float y {}; // position y
};

struct enemy
{
    int health {};
    int speed {};
    int damage {};
    int gold {};
    int x {}; // position en x sur la grille
    int y {}; // position en y sur la grille
    bool is_dead();

    GLuint _knight {};
    GLuint _sorcerer {};
};

struct tower
{
    ProjectileKind projectile;
    int range {};
    int rate {}; // GDN : tir par seconde
    float x {}; // position x
    float y {}; // position y
};

extern int gold_earned; // extern int = idem "méthode" mais pour une variable

void damage(enemy enemy, projectile projectile);
bool in_range(enemy enemy, tower tour);
void fire(enemy enemy, tower tour);

struct node
{
    int node_id;
    int node_x;
    int node_y;
};

std::vector<node> create_vect_nodes(std::vector<std::vector<std::string>> splitted_itd_file);
std::vector<int> get_shortest_path (std::unordered_map <int, std::pair<float, int>> dij_map, std::vector<node> vector_of_nodes);
std::vector<node> get_enemy_path (std::vector<node> vector_of_nodes, std::vector<int> shortest_path);

struct King
{
    int health{};
    int x {}; // position en x sur la grille
    int y {}; // position en y sur la grille
    bool is_dead{0};

    GLuint _king{};

    void draw_king();
};