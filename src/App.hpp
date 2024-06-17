#pragma once
#include <glad/glad.h>
#include <simpletext.h>

#include "code/entities/entities.hpp"
#include "code/draw/draw.hpp"
#include "code/ui/button.hpp"
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <vector>
#include "code/screens/screens.hpp"
#include "code/waves/waves.hpp"

enum state_screen {MENU, screen_LEVEL, screen_LOOSE, screen_WIN, screen_PAUSE};

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
    GLuint _texture; 
    GLuint start_button; 
    GLuint quit_button; 
    GLuint pause_bouton;
    GLuint title_button; 
    GLuint resume_button;
    GLuint retry_button;
    GLuint victory_button;
    GLuint defeat_button;

    double time_open_window = {};
    double time_play = {};

    bool window_close = false;

    float xTower{};
    float yTower{};

    std::vector<tower> towers;
    std::vector<std::pair<float, float>> towers_already_builds{};

    float xBuild{};
    float yBuild{};
    GLuint case_color;

    Map map;
private:
    void render();
    
    int _width {};
    int _height {};
    double _previousTime {};
    float _viewSize {};

    // Buttons
        std::vector<Button> listeDeButton;

    // Map
        // Map map;

    // Entities
        King kinger{kinger.default_health, 0, 0, 0, 100};
        Enemy Purrsival{50, 1, 20, 10, 0, 0, 0};
        Enemy Excalipurr{50, 1, 20, 10, 0, 0, 0};
        Enemy Meowlin{40, 3, 40, 20, 0, 0, 0};
        tower arrow{ProjectileKind::Arrow, 2, 4, 0, 0, 200};

    // Text
        SimpleText TextRenderer {};
        SimpleText player_gold_text;

    // Waves
        Wave current_wave{};
        Wave wave_one {1, {Purrsival, Excalipurr}, 5, 10};
        // Wave wave_two {2, {Purrsival}, 30, 5};
        // Wave wave_three {3, {Purrsival}, 45, 2};
        // Wave wave_four {4, {Purrsival}, 60, 1};
};