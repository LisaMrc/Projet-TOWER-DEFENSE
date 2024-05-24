#include <iostream>
#pragma once


struct start_button {

};

struct quit_button{

};

struct pause_button{

};

enum state { start_button, quit_button, pause_button };

struct screen_button{
    state _state {start_button};
    int nbrTileSide {20};
};

