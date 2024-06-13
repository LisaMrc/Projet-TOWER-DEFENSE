#pragma once
#include <iostream>
#include <glad/glad.h>
#include <simpletext.h>
#include "code/screens/screens.hpp"
#include "code/entities/entities.hpp"
#include "code/draw/draw.hpp"
#include "code/entities/entities.hpp"
#include "code/ui/button.hpp"
#include <GLFW/glfw3.h>

class App {
public:
    App();

    void setup();
    void update();
    
    // GLFW callbacks binding
    void key_callback(int key, int scancode, int action, int mods);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void scroll_callback(double xoffset, double yoffset);
    void cursor_position_callback(double xpos, double ypos);
    void size_callback(int width, int height);

private:
    void render();

    int _width {};
    int _height {};
    double _previousTime {};
    float _viewSize {};

    // Add your variables here
    float _angle {};
    SimpleText TextRenderer {};

    Map map;
    King kinger{250, 0, 0, 0};
    Enemy Purrsival{50, 1, 20, 10, 0, 0, 0};

    tower arrow{ProjectileKind::Arrow, 2, 4, 0, 0, 200};

    screen_button start{state_button::start_button, 20};
    screen_button stop{state_button::quit_button, 20};
    screen_button pause{state_button::pause_button, 20};
};


