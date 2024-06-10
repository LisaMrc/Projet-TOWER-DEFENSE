#pragma once
#include <vector>
#include <string>

// #include "../draw/draw.hpp"

enum class ProjectileKind {
    Arrow,
    Fireball,
    Magicball,
};

struct projectile
{
    int dammages {};
    float speed {};
    float x {}; // position x
    float y {}; // position y
};

struct ennemi
{
    int health {};
    int speed {};
    int damage {};
    int gold {};
    float x {}; // position x
    float y {}; // position y
    bool is_dead();
    
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

void damage(ennemi ennemi, projectile projectile);
bool in_range(ennemi ennemi, tower tour);
void fire(ennemi ennemi, tower tour);

// struct node
// {
//     int node_id;
//     int node_x;
//     int node_y;
// };

// std::vector<node> create_vect_nodes(std::vector<std::vector<std::string>> splitted_itd_file);
