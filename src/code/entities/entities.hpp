#pragma once
#include <vector>
#include <string>

// #include "../draw/draw.hpp"

struct ennemi
{
    int health {};
    int speed {};
    int damage {};
    int gold {};
    float x {}; // position x
    float y {}; // position y
    void damage(projectile projectile);
    bool is_dead();
};

struct tower
{
    int power {};
    int range {};
    int rate {}; // GDN : tir par seconde
};

struct projectile
{
    int dammages {};
    float speed {};
    float x {}; // position x
    float y {}; // position y
};

extern int gold_earned; // extern int = idem "méthode" mais pour une variable


// struct node
// {
//     int node_id;
//     int node_x;
//     int node_y;
// };

// std::vector<node> create_vect_nodes(std::vector<std::vector<std::string>> splitted_itd_file);
