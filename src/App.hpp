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

    bool isWithinRange(const tower& tour, const Enemy& enemy);
    Projectile createProjectile(const tower& tour, Enemy& enemy, ProjectileKind kind);
    

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
    GLuint wood_arrow_button;
    GLuint elec_arrow_button;
    GLuint toggle_button;

    GLuint knight_enemy;
    GLuint wizard_enemy;

    double time_open_window = {};
    double time_play = {};

    bool window_close = false;

    float xTower{};
    float yTower{};

    std::vector<tower> normal_towers;
    std::vector<tower> elec_towers;
    std::vector<std::pair<float, float>> normal_towers_already_builds{};
    std::vector<std::pair<float, float>> elec_towers_already_builds{};
    
    std::vector<Enemy> enemies;

    float xBuild{};
    float yBuild{};
    GLuint case_color;

    ProjectileKind type_projectile{};

    Map map;
    // Buttons
        std::vector<Button> listeDeButton;

    //Entities 
        tower normal_arrow_tower{ProjectileKind::Arrow, 2, 4, 0, 0, 100};
        tower elec_arrow_tower{ProjectileKind::Arrow, 2, 4, 0, 0, 200};
        Player player{500};

private:
    void render();
    
    int _width {};
    int _height {};
    double _previousTime {};
    float _viewSize {};

    // Entities
        King kinger{250, 0, 0, 0, 100};

        tower arrow_tower{ProjectileKind::Arrow, 2, 4, 0, 0, 200};
        tower electric_tower{ProjectileKind::Lightning_arrow, 1, 1, 0, 0, 300};

        Projectile arrow{ProjectileKind::Arrow, 10, 5, 0, 0};
        Projectile ligthening_arrow{ProjectileKind::Lightning_arrow, 40, 10, 0, 0};

        GLuint projectile1_texture;
        GLuint projectile2_texture;

    // Text
        SimpleText TextRenderer {};
        SimpleText player_gold_text;

    // Waves
        std::vector<Wave> waves_list{};
        Wave current_wave{};
};