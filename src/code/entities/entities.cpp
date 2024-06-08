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

