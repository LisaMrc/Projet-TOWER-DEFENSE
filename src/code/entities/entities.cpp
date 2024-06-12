#include "entities.hpp"
#include "../draw/draw.hpp"
#include "../draw/sil.hpp"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <stack>

int gold_earned {0};

bool enemy::is_dead() {
    if (health <= 0) {
        gold_earned += gold;
        return true;
    } else {
        return false;
    }
}

void damage(enemy enemy, projectile projectile)
{
    if (projectile.x == enemy.x && projectile.y == enemy.y) {
        enemy.health -= projectile.damages;
    }
}

bool in_range(enemy enemy, tower tour)
{
    if (enemy.x - tour.x <= tour.range || enemy.y - tour.y <= tour.range) {
        return true;
    }
}

void fire(enemy enemy, tower tour)
{
    bool range {in_range(enemy, tour)};
    bool death {enemy.is_dead()};
    while (range == true || death == false) {
        range = in_range(enemy, tour);
        death = enemy.is_dead();
        // envoyer un projectile (== projectileKind) toutes les XX secondes (== tower.rate) sur l'enemy
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

// std::vector<node> enemy_move(std::vector<node> enemy_path)
// {
//     translate enemy from path to path
// }

// void King::draw_king()
// {
    
// }

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

void create_tower(Map &map, tower &tour) {
    gold_earned -= tour.price;
}

