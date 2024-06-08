#include <iostream>
#pragma once

struct ennemi
{
    int health {};
    int speed {};
    int damage {};
    int gold {};

    // bool is_hitting();
    // void damage();
    bool is_dead();
};



struct tower
{
    int power {};
    int range {};
    int rate {}; // GDN : tir par seconde
};

extern int gold_earned; // extern int = idem "méthode" mais pour une variable