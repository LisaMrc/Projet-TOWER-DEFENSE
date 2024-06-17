#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include "../draw/sil.hpp"
#include <GLHelpers.hpp>
#include "../draw/draw.hpp"

// #include "../draw/draw.hpp"
extern int gold_earned; // extern int = idem "méthode" mais pour une variable

struct clickPos
{
    float x{};
    float y{};
};

struct node
{
    int node_id;
    float node_x;
    float node_y;
};

enum class EnemyType
{
    KNIGHT,
    WIZARD,
};

struct Enemy
{
    int enemy_id;
    bool is_dead{};

    float x {};
    float y {};

    int health {};
    int speed {};
    int damage {};
    int gold {};

    EnemyType type{};
    GLuint texture {};

    std::vector<node> enemy_path;
    int current_node_id{};
    int target_node_id{};
    double enemy_clock;
    
    void enemy_move();
    void get_elapsedTime(double const & elapsedTime);
    void oof();
    void reset();
};

std::vector<node> create_vect_nodes(std::vector<std::vector<std::string>> splitted_itd_file);
std::vector<int> get_shortest_path (std::unordered_map <int, std::pair<float, int>> dij_map, std::vector<node> vector_of_nodes);
std::vector<node> get_enemy_path (std::vector<node> vector_of_nodes, std::vector<int> shortest_path);

struct King
{
    int health{};
    float x {};
    float y {};
    bool is_dead{0};

    int player_gold;

    std::vector<node> enemy_path;

    GLuint _king{};

    void reset();
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

struct Player
{
    int gold;

    void analyses_ennemies(std::vector<Enemy> ennemies_in_wave);
};