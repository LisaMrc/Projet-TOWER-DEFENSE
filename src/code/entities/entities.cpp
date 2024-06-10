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
// }