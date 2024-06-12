#pragma once

#include <glad/glad.h>
#include <simpletext.h>
#include "code/screens/screens.hpp"
#include "code/entities/entities.hpp"
#include "code/ui/button.hpp"
#include <vector>
#include <unordered_map>

enum state_screen {MENU, screen_LEVEL};

class App {
public:
    App();

    void setup();
    void update();
    
    // GLFW callbacks binding
    void key_callback(int key, int scancode, int action, int mods);
    void mouse_button_callback(int button, int action, int mods);
    void scroll_callback(double xoffset, double yoffset);
    void cursor_position_callback(double xpos, double ypos);
    void size_callback(int width, int height);

    state_screen _state = state_screen::MENU;
    int windowWidth;
    int windowHeight;

    bool mouse_is_pressed = false;

    int mouseXpos;
    int mouseYpos;

    bool window_close = false;

private:
    void render();



    int _width {};
    int _height {};
    double _previousTime {};
    float _viewSize {};

    // Add your variables here
    GLuint _texture {};
    float _angle {};

    std::vector<Button> listeDeButton;

    SimpleText TextRenderer {};
};