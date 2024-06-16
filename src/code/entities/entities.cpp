#include "entities.hpp"
#include "../draw/draw.hpp"
#include "../draw/sil.hpp"
#include "../../App.hpp"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <stack>

// Initialise la somme d'argent en début de partie
int gold_earned {10};

void Enemy::oof()
{
    if (health <= 0)
    {
        is_dead == true;
    }
}

void damage(Enemy Enemy, projectile projectile)
{
    if (projectile.x == Enemy.x && projectile.y == Enemy.y) {
        Enemy.health -= projectile.damages;
    }
}

bool in_range(Enemy Enemy, tower tour)
{
    if (Enemy.x - tour.x <= tour.range || Enemy.y - tour.y <= tour.range) {
        return true;
    }
}

// void fire(Enemy Enemy, tower tour)
// {
//     bool range {in_range(Enemy, tour)};
//     bool death {Enemy.is_dead()};

//     while (range == true || death == false)
//     {
//         range = in_range(Enemy, tour);
//         death = Enemy.is_dead();
//         // envoyer un projectile (== projectileKind) toutes les XX secondes (== tower.rate) sur l'Enemy
//     }
// }

std::vector<node> create_vect_nodes(std::vector<std::vector<std::string>> splitted_itd_file)
{
    std::vector<node> vect_nodes{};

    for (std::vector<std::string> splitted_line : splitted_itd_file)
    {
        if (splitted_line[0] == "node")
        {
            node current_node = {stoi(splitted_line[1]), stoi(splitted_line[2]), stoi(splitted_line[3])};
            vect_nodes.push_back(current_node);
        }
    }
    return vect_nodes;
}

std::vector<int> get_shortest_path (const std::unordered_map<int, std::pair<float, int>> dij_map, std::vector<node> vector_of_nodes)
{
    std::vector<int> shortest_path_vec;
    std::stack<int> s;
    int current = vector_of_nodes.back().node_id;

    while (current != vector_of_nodes.front().node_id)
    {
        s.push(current);
        current = dij_map.at(current).second;
    }

    s.push(vector_of_nodes.front().node_id);

    while (!s.empty())
    {
        shortest_path_vec.push_back(s.top());
        s.pop();
    }

    return shortest_path_vec;
}

std::vector<node> get_enemy_path (std::vector<node> vector_of_nodes, std::vector<int> shortest_path)
{
    std::vector<node> enemy_path{};

    for (int node : shortest_path)
    {
        enemy_path.push_back(vector_of_nodes[node]);
    }

    return enemy_path;
}

void create_tower(Map &map, tower &tour, float x, float y) {
    bool constructible{map.can_create_tower(map, x, y)};
    if (constructible == true) {
        gold_earned -= tour.price;
        draw_quad_with_texture(tour._arrow, x, y, map);
    } else {
        // std::cout << "Il n'est pas possible de construire cette tour" << std::endl;
    }
    
}

void Enemy::get_elapsedTime (const double & elapsedTime)
{
    this->enemy_clock = elapsedTime;
}

void Enemy::enemy_move()
{
    node current_node{this->enemy_path[this->current_node_id]};
    node target_node{this->enemy_path[this->target_node_id]};

        int coeff_x{1};
        int coeff_y{1};

        if (target_node.node_x < current_node.node_x)
            coeff_x = -1;

        if (target_node.node_y < current_node.node_y)
            coeff_y = -1;

        if (abs(target_node.node_x - current_node.node_x) > abs(target_node.node_y - current_node.node_y))
        {
            if (std::round(this->x * 10) / 10 == target_node.node_x)
            {
                this->target_node_id++;
                this->current_node_id++;
            }
            else
                this->x += coeff_x * this->enemy_clock * this->speed;
        }
        else
        {
            if (std::round(this->y * 10) / 10 == target_node.node_y)
            {
                this->target_node_id++;
                this->current_node_id++;
            }
            else
                this->y += coeff_y * this->enemy_clock * this->speed;
        }
}

void Player::analyses_ennemies(std::vector<Enemy> ennemies_in_wave)
{
    for (Enemy enemy : ennemies_in_wave)
    {
        if (enemy.is_dead = 1)
        {
            gold+= enemy.gold;
        }
    }  
}