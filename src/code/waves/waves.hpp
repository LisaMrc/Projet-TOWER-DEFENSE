#include <vector>

#include "../entities/entities.hpp"

struct Wave
{
    int id;
    std::vector<Enemy> enemies_in_wave;
    int freq_btw_ennemies_in_s;
    int start;

    int nbr_knights;
    int nbr_wizards;
};