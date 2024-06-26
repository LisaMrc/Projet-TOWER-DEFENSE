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
    bool is_on_stage{};

    float x {};
    float y {};

    int health {};
    float speed {};
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
    void ko();
    void reset();
    void update(double elapsedTime);
    void takeDamage(int damage);
};

std::vector<node> create_vect_nodes(std::vector<std::vector<std::string>> splitted_itd_file);
std::vector<int> get_shortest_path (std::unordered_map <int, std::pair<float, int>> dij_map, std::vector<node> vector_of_nodes);
std::vector<node> get_enemy_path (std::vector<node> vector_of_nodes, std::vector<int> shortest_path);
void damage(Enemy enemy, int damage);

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
    void ko();
};

enum class ProjectileKind
{
    Arrow,
    Fireball,
    Lightning_arrow,
};

struct Projectile
{
    ProjectileKind projectile;
    int damages {};
    float speed {};
    float x {};
    float y {};
    Enemy *target{};

    GLuint _Arrow;
    GLuint _Lightning_arrow;
    void update(double elapsedTime);
    // void move(const double &elapsedTime);
    bool is_enemy_hit();
    bool hasHitTarget() const;
};

struct tower
{
    ProjectileKind projectile;
    float range {};
    int rate {}; // tir par seconde
    float x {};
    float y {};
    int price{};
    GLuint _arrow{};
    std::vector<Projectile> projectiles; // Liste des projectiles tirés
    double lastShotTime;
};

struct Player
{
    int gold;
};

void create_tower(Map &map, tower &tour, float x, float y, Player &player);