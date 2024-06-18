#include <vector>

#include "../entities/entities.hpp"

struct Wave
{
    int id;
    std::vector<Enemy> enemies_in_wave;
    double start;
    double freq_btw_ennemies_in_s;

    int nbr_knights;
    int nbr_wizards;
};