#include <vector>

#include "../entities/entities.hpp"

struct Wave
{
    int nbr;
    std::vector<Enemy> enemies_in_wave;
    int start;
    int freq_btw_ennemies_in_s;
};