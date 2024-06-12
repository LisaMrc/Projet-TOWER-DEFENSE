#include <iostream>
#pragma once


// enum state { start_button, quit_button, pause_button };


// void clic_button_callback(GLFWwindow* window, int button, int action, int mods);

struct Button{

    std::string name;
    bool isPressed {false};
    // en coordonées de case 
    int posX; 
    int posY;
    int width;
    int height;

    GLuint texture;

    void draw_me();
};

