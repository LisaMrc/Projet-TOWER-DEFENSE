#include <iostream>
#pragma once


struct start_button {

};

struct quit_button{
    
};

struct pause_button{
    
};

// enum class state { start_button, quit_button, pause_button };

struct screen_button{
    state _state {};
    int nbrTileSide {};

    GLuint _start {};
    GLuint _pause {};
    GLuint _stop {};
};

