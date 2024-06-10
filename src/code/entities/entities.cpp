#include "entities.hpp"

int gold_earned {0};

bool ennemi::is_dead() {
    if (health <= 0) {
        gold_earned += gold;
        return true;
    } else {
        return false;
    }
}

void damage(ennemi ennemi, projectile projectile) {
    if (projectile.x == ennemi.x && projectile.y == ennemi.y) {
        ennemi.health -= projectile.dammages;
    }
}

bool in_range(ennemi ennemi, tower tour) {
    if (ennemi.x - tour.x <= tour.range || ennemi.y - tour.y <= tour.range) {
        return true;
    }
}

void fire(ennemi ennemi, tower tour) {
    bool range {in_range(ennemi, tour)};
    bool death {ennemi.is_dead()};
    while (range == true || death == false) {
        range = in_range(ennemi, tour);
        death = ennemi.is_dead();
        // envoyer un projectile (== projectileKind) toutes les XX secondes (== tower.rate) sur l'ennemi
    }
}

// std::vector<node> create_vect_nodes(std::vector<std::vector<std::string>> splitted_itd_file)
// {
//     std::vector<node> vect_nodes{};

//     for (std::vector<std::string> splitted_line : splitted_itd_file)
//     {
//         if (splitted_line[0] == "node")
//         {
//             node current_node = {stoi(splitted_line[1]), stoi(splitted_line[2]), stoi(splitted_line[3])};
//             vect_nodes.push_back(current_node);
//         }
//     }
//     return vect_nodes;
// }

// std::vector<node> enemy_map(std::vector<node> vect_nodes, std::unordered_map<int, std::pair<float, int>> dij_map)
// {
//     std::vector<node> enemy_path{};

//     for (std::vector<std::string> splitted_line : splitted_itd_file)
//     {
//         Lire Dijkstra...
//     }

//     return enemy_path;
// }

// std::vector<node> create_vect_nodes(std::vector<std::vector<std::string>> splitted_itd_file)
// {
//     std::vector<node> vect_nodes{};

//     for (std::vector<std::string> splitted_line : splitted_itd_file)
//     {
//         if (splitted_line[0] == "node")
//         {
//             node current_node = {stoi(splitted_line[1]), stoi(splitted_line[2]), stoi(splitted_line[3])};
//             vect_nodes.push_back(current_node);
//         }
//     }
//     return vect_nodes;
// }

// std::vector<node> enemy_move(std::vector<node> enemy_path)
// {
//     translate enemy from path to path
// }#include "entities.hpp"



