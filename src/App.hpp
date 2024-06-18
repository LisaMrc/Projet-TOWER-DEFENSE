#pragma once
#include <glad/glad.h>
#include <simpletext.h>
#include <GLFW/glfw3.h>

#include "code/entities/entities.hpp"
#include "code/draw/draw.hpp"
#include "code/ui/button.hpp"
#include "code/screens/screens.hpp"
#include "code/waves/waves.hpp"

#include <vector>

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
    GLuint hood_arrow_button;
    GLuint elec_arrow_button;

    GLuint knight_enemy;
    GLuint wizard_enemy;

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
    // Buttons
        std::vector<Button> listeDeButton;

    //Entities 
        tower arrow{ProjectileKind::Arrow, 2, 4, 0, 0, 200};
        tower elec_arrow{ProjectileKind::Arrow, 2, 4, 0, 0, 200};

private:
    void render();
    
    int _width {};
    int _height {};
    double _previousTime {};
    float _viewSize {};

    // Entities
        King kinger{250, 0, 0, 0, 100};

    // Text
        SimpleText TextRenderer {};
        SimpleText player_gold_text;

    // Waves
        std::vector<Wave> waves_list{};
        Wave current_wave{};
};