#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "../draw/sil.hpp"
#include <GLHelpers.hpp>
#include "../draw/draw.hpp"

// #include "../draw/draw.hpp"
extern int gold_earned; // extern int = idem "méthode" mais pour une variable


struct clickPos {
    float x{};
    float y{};
};



struct node
{
    int node_id;
    float node_x;
    float node_y;
};

struct Enemy
{
    int health {};
    int speed {};
    int damage {};
    int gold {};

    float x {}; // position en x sur la grille
    float y {}; // position en y sur la grille
    
    bool is_dead{};

    GLuint _knight {};
    GLuint _sorcerer {};

    std::vector<node> enemy_path;
    int current_node_id = 0;
    int target_node_id = 1;
    double enemy_clock;
    void get_elapsedTime(double const & elapsedTime);
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

    int player_gold;

    GLuint _king{};
};

struct projectile
{
    int damages {};
    float speed {};
    float x {};
    float y {};
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
    float x {};
    float y {};
    int price{};

    GLuint _arrow{};
};

void damage(Enemy Enemy, projectile projectile);
bool in_range(Enemy Enemy, tower tour);
void fire(Enemy Enemy, tower tour);
void create_tower(Map &map, tower &tour, float x, float y);