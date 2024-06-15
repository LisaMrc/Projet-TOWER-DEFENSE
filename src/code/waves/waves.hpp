#include <vector>

#include "../entities/entities.hpp"

struct Wave
{
    int nbr;
    std::vector<Enemy> vector_of_ennemies;
    int start;
    int freq_btw_ennemies_in_s;
};