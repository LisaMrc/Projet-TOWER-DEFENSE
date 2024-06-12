#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include <GLHelpers.hpp>

// #include "../draw/draw.hpp"
extern int gold_earned; // extern int = idem "méthode" mais pour une variable

struct node
{
    int node_id;
    int node_x;
    int node_y;
};

struct Enemy
{
    int health {};
    int speed {};
    int damage {};
    int gold {};

    float x {}; // position en x sur la grille
    float y {}; // position en y sur la grille
    
    bool is_dead();

    GLuint _knight {};
    GLuint _sorcerer {};

    std::vector<node> enemy_path;
    double enemy_clock;
    void get_elapsedTime(double const & elapsedTime);
    int node_nbr = 1;
    void enemy_move();
};

std::vector<node> create_vect_nodes(std::vector<std::vector<std::string>> splitted_itd_file);
std::vector<int> get_shortest_path (std::unordered_map <int, std::pair<float, int>> dij_map, std::vector<node> vector_of_nodes);
std::vector<node> get_enemy_path (std::vector<node> vector_of_nodes, std::vector<int> shortest_path);

struct King
{
    int health{};
    float x {}; // position en x sur la grille
    float y {}; // position en y sur la grille
    bool is_dead{0};

    GLuint _king{};
};

struct projectile
{
    int damages {};
    float speed {};
    float x {}; // position x
    float y {}; // position y
};

enum class ProjectileKind
{
    Arrow,
    Fireball,
    lightning_arrow,
};

struct tower
{
    ProjectileKind projectile;
    int range {};
    int rate {}; // tir par seconde
    float x {}; // position x
    float y {}; // position y
};

void damage(Enemy Enemy, projectile projectile);
bool in_range(Enemy Enemy, tower tour);
void fire(Enemy Enemy, tower tour);