#include <iostream>
#pragma once

enum state { MAIN_MENU, LEVEL, PAUSE_MENU, LEVEL_SELECTOR };

struct screen{
    state _state {LEVEL};
    int nbrTileSide {20};
};


