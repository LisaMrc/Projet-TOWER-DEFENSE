#include <iostream>
#pragma once

struct knight {
    int health {200};
    int speed {8};
    // int dammage {50};
    int gold {30};
};

struct wizzard {
    int health {80};
    int speed {4};
    // int dammage {100};
    int gold {60};
};

struct tower {
    int power {20};
    int range {10};
    int rate {1}; // tir par seconde
}

int gold_earned{0};