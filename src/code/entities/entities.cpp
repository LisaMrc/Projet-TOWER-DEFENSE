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

// _________________________________________ GRAPH FOR ENEMIES + ENEMIES PATH ____________________________________________________

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

// __________________________________________ ENEMY ___________________________________________________

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

void Enemy::ko()
{
    if (health <= 0)
    {
        is_dead = true;
        is_on_stage = false;
    }
}

void Enemy::reset()
{
    x = enemy_path.front().node_x;
    y = enemy_path.front().node_y;
    current_node_id = 0;
    target_node_id = 1;
}

void Player::analyses_ennemies(std::vector<Enemy> ennemies_in_wave)
{
    for (Enemy enemy : ennemies_in_wave)
    {
        if (enemy.is_dead == 1)
        {
            gold+= enemy.gold;
        }
    }  
}

void damage(Enemy enemy, int damage)
{
    enemy.health -= damage;
    if (enemy.health <= 0) {
        enemy.is_dead = true;
    }
}

// __________________________________________ KING ___________________________________________________

void King::reset()
{
    x = enemy_path.back().node_x;
    y = enemy_path.back().node_y;
    health = 250;
    is_dead = 0;
}

void King::ko()
{
    if (health <= 0)
    {
        is_dead = true;
    }
}

// __________________________________________ TOWERS ___________________________________________________

void create_tower(Map &map, tower &tour, float x, float y)
{
        gold_earned -= tour.price;
        tour.lastShotTime = 0.0;
        draw_quad_with_texture(tour._arrow, x, y, map);   
}

bool in_range(Enemy enemy, tower tour) {
    float dx = tour.x - enemy.x;
    float dy = tour.y - enemy.y;
    float distance = sqrt(dx * dx + dy * dy); // Correction ici
    return distance <= tour.range;
}


// void fire(Enemy enemy, tower tour)
// {
//     bool range {in_range(enemy, tour)};
//     bool death {enemy.is_dead};

//     while (range == true || death == false)
//     {
//         range = in_range(enemy, tour);
//         death = enemy.is_dead;
//         auto now {std::chrono::steady_clock::now()};
//         std::chrono::duration<double> elapsed = now - tour.lastFireTime;
//         if (elapsed.count() >= tour.rate) {
//             if (tour.projectile == ProjectileKind::Arrow) {
//                 Projectile newProjectile{ProjectileKind::Arrow, 10, 5, tour.x, tour.y, enemy};
//                 tour.projectiles.push_back(newProjectile);
//             }
//             else if (tour.projectile == ProjectileKind::Lightning_arrow) {
//                 Projectile newProjectile{ProjectileKind::Lightning_arrow, 15, 7, tour.x, tour.y, enemy};
//                 tour.projectiles.push_back(newProjectile);
//             }
//         tour.lastTime = now;
//     }
//     }
// }

// bool Projectile::is_enemy_hit() {
//     if (cible.x == x || cible.y == y) {
//         return true;
//     } else {
//         return false;
//     }
// }

// void Projectile::move(const double & elapsedTime) {
//     float dx = cible.x - x;
//     float dy = cible.y - y;
//     float distance = sqrt(dx * dx + dy * dy);

//     if (distance < speed * elapsedTime) {
//         x = cible.x;
//         y = cible.y;
//     } else {
//         x += dx / distance * speed * elapsedTime;
//         y += dy / distance * speed * elapsedTime;
//     }
// }


void Enemy::update(double elapsedTime) {
    // Mise à jour de la position de l'ennemi
}

void Projectile::update(double elapsedTime) {
    // Mise à jour de la position du projectile
    // Calcul de la direction vers l'ennemi cible
    float dx = target.x - x;
    float dy = target.y - y;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance < speed * elapsedTime) {
        x = target.x;
        y = target.y;
    } else {
        x += dx / distance * speed * elapsedTime;
        y += dy / distance * speed * elapsedTime;
    }
}

bool Projectile::hasHitTarget() const {
    return x == target.x && y == target.y;
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        is_dead = true;
    }
}
