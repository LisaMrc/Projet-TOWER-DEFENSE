#include "entities.hpp"
#include "../draw/draw.hpp"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <stack>

int gold_earned {0};

bool Enemy::is_dead() {
    if (health <= 0) {
        gold_earned += gold;
        return true;
    } else {
        return false;
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

void fire(Enemy Enemy, tower tour)
{
    bool range {in_range(Enemy, tour)};
    bool death {Enemy.is_dead()};
    while (range == true || death == false) {
        range = in_range(Enemy, tour);
        death = Enemy.is_dead();
        // envoyer un projectile (== projectileKind) toutes les XX secondes (== tower.rate) sur l'Enemy
    }
}

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

void Enemy::enemy_move()
{
    node start_of_path = this->enemy_path[0];
    // node end_of_path = this->enemy_path.back();
    node end_of_path = this->enemy_path[1];

    node current_node{start_of_path};
    node target_node{this->enemy_path[1]};

    if (this->x < 6)
    {
        this->x += .01*this->speed;
    }
    else
    {
        this->x += 0;
    }

    // while ((this->x != end_of_path.node_x) && (this->y != end_of_path.node_y))
    // {
    //     this->x += .01*this->speed;
    // }
}