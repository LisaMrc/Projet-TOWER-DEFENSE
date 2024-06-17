#include <iostream>
#include "App.hpp"
#include "code/draw/draw.hpp"
#include "code/entities/entities.hpp"
#include "code/ui/button.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>

#include "simpletext.h"
#include "utils.hpp"
#include "GLHelpers.hpp"

App::App() : _previousTime(0.0), _viewSize(2.0)
{
    img::Image test {img::load(make_absolute_path("images/level.png", true), 4, true)};

    // LAND TEXTURES
    img::Image grass {img::load(make_absolute_path("images/textures/land/grass.bmp", true), 4, true)};
    img::Image path {img::load(make_absolute_path("images/textures/land/path.bmp", true), 4, true)};
    img::Image in {img::load(make_absolute_path("images/textures/land/in.png", true), 4, true)};
    img::Image out {img::load(make_absolute_path("images/textures/land/out.png", true), 4, true)};

    map._grass = loadTexture(grass);
    map._path = loadTexture(path);
    map._in = loadTexture(in);
    map._out = loadTexture(out);
    
    // ENTITIES TEXTURES
    img::Image king {img::load(make_absolute_path("images/textures/entities/king.png", true), 4, true)};
    img::Image knight {img::load(make_absolute_path("images/textures/entities/knight.png", true), 4, true)};
    img::Image wizard {img::load(make_absolute_path("images/textures/entities/wizard.png", true), 4, true)};
    img::Image tower {img::load(make_absolute_path("images/textures/entities/tower_1.png", true), 4, true)};

    kinger._king = loadTexture(king);

    Purrsival._knight = loadTexture(knight);
    Excalipurr._knight = loadTexture(knight);
    Meowlin._wizard = loadTexture(wizard);

    arrow._arrow = loadTexture(tower);

    // BUTTONS TEXTURE
    img::Image start {img::load(make_absolute_path("images/textures/buttons/start_button.png", true), 4, true)};
    img::Image quit {img::load(make_absolute_path("images/textures/buttons/quit_button.png", true), 4, true)};
    img::Image pause {img::load(make_absolute_path("images/textures/buttons/pause_button.png", true), 4, true)};
    img::Image resume {img::load(make_absolute_path("images/textures/buttons/resume_button.png", true), 4, true)};
    img::Image retry {img::load(make_absolute_path("images/textures/buttons/retry_button.png", true), 4, true)};
    img::Image title {img::load(make_absolute_path("images/textures/buttons/title.png", true), 4, true)};
    img::Image victory {img::load(make_absolute_path("images/textures/buttons/victory_button.png", true), 4, true)};
    img::Image defeat {img::load(make_absolute_path("images/textures/buttons/defeat_button.png", true), 4, true)};
    img::Image hood_arrow {img::load(make_absolute_path("images/textures/entities/tower_1.png", true), 4, true)};
    img::Image elec_arrow {img::load(make_absolute_path("images/textures/entities/tower_2.png", true), 4, true)};

    start_button = loadTexture(start);
    quit_button = loadTexture(quit);
    pause_bouton = loadTexture(pause);
    title_button = loadTexture(title);
    resume_button = loadTexture(resume);
    retry_button = loadTexture(retry);
    victory_button = loadTexture(victory);
    defeat_button = loadTexture(defeat);
    hood_arrow_button = loadTexture(hood_arrow);
    elec_arrow_button = loadTexture(elec_arrow);

    // TOWER PLACEMENT
    img::Image free {img::load(make_absolute_path("images/textures/zones_tours/zone_verte.png", true), 4, true)};
    img::Image occupied {img::load(make_absolute_path("images/textures/zones_tours/zone_rouge.png", true), 4, true)};

    map._free = loadTexture(free);
    map._occupied = loadTexture(occupied);
}

void App::setup()
{
    // Set the background color to black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Setup the text renderer with blending enabled and white text color
    TextRenderer.ResetFont();
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::WHITE);
    TextRenderer.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    TextRenderer.EnableBlending(true);

    // Initializes player_gold_text
    this->player_gold_text.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::WHITE);
    this->player_gold_text.SetTextSize(SimpleText::FontSize::SIZE_32);
    this->player_gold_text.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    this->player_gold_text.EnableBlending(true);

    // Initializes the buttons
    listeDeButton.push_back(Button{"Boutton_Start", false, 3, 4, 2, 1, start_button}); //0
    listeDeButton.push_back(Button{"Boutton_Quit", false, 3, 6, 2, 1, quit_button}); //1
    listeDeButton.push_back(Button{"Boutton_Pause", false, 8, 0, 2, 1, pause_bouton}); //2
    listeDeButton.push_back(Button{"Boutton_Titre", false, 3, 1, 6, 2, title_button}); //3
    listeDeButton.push_back(Button{"Boutton_Loose", false, 3, 1, 6, 2, defeat_button}); //4
    listeDeButton.push_back(Button{"Boutton_Win", false, 3, 1, 6, 2, victory_button}); //5
    listeDeButton.push_back(Button{"Boutton_Play_again", false, 3, 4, 2, 1, retry_button}); //6
    listeDeButton.push_back(Button{"Boutton_Play", false, 3, 4, 2, 1, resume_button}); //7
    listeDeButton.push_back(Button{"Boutton_Hood_Arrow", false, -3, 2, 1, 1, hood_arrow_button}); //8
    listeDeButton.push_back(Button{"Boutton_Elec_Arrow", false, -2, 2, 1, 1, elec_arrow_button}); //9

    // Extract information from itd file
    std::vector<std::vector<std::string>> splitted_itd_file = split_itd_file();

    // Creates the tools to print the map
    map.associate_RGB_to_CaseType(splitted_itd_file);
    map.associate_px_pos_to_CaseType();

    // Create graph for ennemies from itd
    std::vector<std::vector<float>> adjacency_matrix {create_adjacency_matrix(splitted_itd_file)};
    Graph::WeightedGraph graph {Graph::build_from_adjacency_matrix(adjacency_matrix)};
    
    // Create graph for ennemies
    std::unordered_map<int, std::pair<float, int>> dij_map = graph.dijkstra(0, 7); 
    std::vector<node> vec_nodes = create_vect_nodes(splitted_itd_file);
    std::vector<int> shortest_path = get_shortest_path (dij_map, vec_nodes);
    std::vector<node> enemy_path = get_enemy_path (vec_nodes, shortest_path);

    // Initializes enemy_path in each enemy
    Purrsival.enemy_path = enemy_path;
    Excalipurr.enemy_path = enemy_path;
    Meowlin.enemy_path = enemy_path;

    kinger.enemy_path = enemy_path;
}

void App::update()
{
    const double currentTime { glfwGetTime() };
    const double elapsedTime { currentTime - _previousTime};
    _previousTime = currentTime;

    // if start is pressed
    if (listeDeButton[0].isPressed)
    {
        // Initialise le roi (Kinger) 
        kinger.reset();

        // Initialise l'ennemi 1 (Purrsival)
        Purrsival.reset();

        // Initialise l'ennemi 2 (Excalipurr)
        Excalipurr.reset();

        // Initializes the wave
        current_wave = wave_one;
    
        _state = state_screen::screen_LEVEL;
        time_open_window = {glfwGetTime()};
    }

    if (listeDeButton[1].isPressed) // if quit is pressed
    {
        window_close = true;
    }
    
    if (listeDeButton[2].isPressed) //if pause is pressed
    {
        _state = state_screen::screen_PAUSE;
    }

    if (listeDeButton[7].isPressed) //if resume play is pressed
    {
        _state = state_screen::screen_LEVEL;
    }

    if(listeDeButton[6].isPressed) //if play again is pressed
    {
        _state = state_screen::MENU;
    }

    if (state_screen::screen_LEVEL)
    {
        // KING
        if (kinger.health <= 0)
        {
            kinger.is_dead = 1;
        }

        if (Purrsival.current_node_id == Purrsival.enemy_path.back().node_id)
        {
            kinger.health -= Purrsival.damage;
        }

        // ENEMY
        Purrsival.get_elapsedTime(elapsedTime);
        Purrsival.oof();

        Excalipurr.get_elapsedTime(elapsedTime);
        Excalipurr.oof();
    }

    render();
}

void App::render()
{
    // Clear the color and depth buffers of the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLoadIdentity();

    // Render the text          
    TextRenderer.Render();

    if(_state == state_screen::screen_LEVEL)
    {
        // Gets variables
        time_play = glfwGetTime();

        // Change buttons state
        listeDeButton[0].isPressed = false;
        listeDeButton[7].isPressed = false;

        // RENDERS IMMEDIATELY

            // Render pause button
            listeDeButton[2].draw_me();

            // Render types towers button
            listeDeButton[8].draw_me(); //normal tower
            listeDeButton[9].draw_me(); //electrical tower

            // Render the map
            map.draw_map(map);

            // Render the King
            draw_quad_with_texture(kinger._king, kinger.x, kinger.y, map);

            // Render player's gold number
            std::string GOLD_Label{" GOLD : " + std::to_string(kinger.player_gold) + " "};
            this->player_gold_text.Label(GOLD_Label.c_str() , _width / 80, 100, SimpleText::LEFT);
            this->player_gold_text.Render();
        // 

        // TRIGGERS (awaits for...)
            
            if (kinger.is_dead == 1)
            {
                _state = state_screen::screen_LOOSE;
            }
            else if (time_play > 120+time_open_window && kinger.health != 0)
            {
                _state = state_screen::screen_WIN;
            }

            if (time_play > current_wave.freq_btw_ennemies_in_s)
            {
                // Render the first knight
                if (Purrsival.target_node_id < Purrsival.enemy_path.size())
                {
                    Purrsival.enemy_move();
                }
                draw_quad_with_texture(Purrsival._knight, Purrsival.x, Purrsival.y, map);
            }

            if (time_play > current_wave.freq_btw_ennemies_in_s*2)
            {
                // Render the second knight
                if (Excalipurr.target_node_id < Excalipurr.enemy_path.size())
                {
                    Excalipurr.enemy_move();
                }
                draw_quad_with_texture(Excalipurr._knight, Excalipurr.x, Excalipurr.y, map);
            }

            if (listeDeButton[8].isPressed){    
                listeDeButton[9].isPressed = false;
                for (const auto& tower : towers){
                    create_tower(map, arrow, tower.x, tower.y);
                }
            }

            if (listeDeButton[9].isPressed){
                listeDeButton[8].isPressed = false;
                for (const auto& tower : towers){
                    create_tower(map, arrow, tower.x, tower.y);
                }
            }
        
        draw_quad_with_texture(case_color, xBuild, yBuild, map);
    }

    if(_state == state_screen::MENU)
    {
        listeDeButton[0].isPressed = false;
        listeDeButton[1].isPressed = false;
        listeDeButton[2].isPressed = false;
        listeDeButton[6].isPressed = false;
        listeDeButton[8].isPressed = false;
        listeDeButton[9].isPressed = false;
        listeDeButton[0].draw_me();
        listeDeButton[1].draw_me();
        listeDeButton[3].draw_me();
    }

    if (_state == state_screen::screen_PAUSE)
    {
        listeDeButton[8].isPressed = false;
        listeDeButton[9].isPressed = false;
        listeDeButton[2].isPressed = false;
        listeDeButton[1].draw_me();
        listeDeButton[7].draw_me();
    }

    if (_state == state_screen::screen_LOOSE)
    {
        listeDeButton[4].draw_me();
        listeDeButton[6].draw_me();
        listeDeButton[1].draw_me();
    }

    if (_state == state_screen::screen_WIN)
    {
        listeDeButton[5].draw_me();
        listeDeButton[6].draw_me();
        listeDeButton[1].draw_me();
    }
}

void App::key_callback(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/) {
}

void App::mouse_button_callback(int button, int action, int mods) {
    if(mouseXpos >= listeDeButton[0].posX && mouseXpos < listeDeButton[0].posX+listeDeButton[0].width && 
    mouseYpos >= listeDeButton[0].posY && mouseYpos < listeDeButton[0].posY + listeDeButton[0].height && _state == state_screen::MENU){
        listeDeButton[0].isPressed = true;
    }
    if(mouseXpos >= listeDeButton[1].posX && mouseXpos < listeDeButton[1].posX+listeDeButton[1].width && 
    mouseYpos >= listeDeButton[1].posY && mouseYpos < listeDeButton[1].posY + listeDeButton[1].height && (_state == state_screen::MENU||_state == state_screen::screen_PAUSE||_state == state_screen::screen_LOOSE||_state == state_screen::screen_WIN)){
        listeDeButton[1].isPressed = true;
    }
    if(mouseXpos >= listeDeButton[2].posX && mouseXpos < listeDeButton[2].posX+listeDeButton[2].width && 
    mouseYpos >= listeDeButton[2].posY && mouseYpos < listeDeButton[2].posY + listeDeButton[2].height && _state == state_screen::screen_LEVEL){
        listeDeButton[2].isPressed = true;
    }
    if(mouseXpos >= listeDeButton[6].posX && mouseXpos < listeDeButton[6].posX+listeDeButton[6].width && 
    mouseYpos >= listeDeButton[6].posY && mouseYpos < listeDeButton[6].posY + listeDeButton[6].height && (_state == state_screen::screen_LOOSE||_state == state_screen::screen_WIN)){
        listeDeButton[6].isPressed = true;
    }
    if(mouseXpos >= listeDeButton[7].posX && mouseXpos < listeDeButton[7].posX+listeDeButton[7].width && 
    mouseYpos >= listeDeButton[7].posY && mouseYpos < listeDeButton[7].posY + listeDeButton[7].height && (_state == state_screen::screen_PAUSE)){
        listeDeButton[7].isPressed = true;
    }
    if(mouseXpos >= listeDeButton[8].posX+1 && mouseXpos < listeDeButton[8].posX+listeDeButton[8].width+1 && 
    mouseYpos >= listeDeButton[8].posY && mouseYpos < listeDeButton[8].posY + listeDeButton[8].height && _state == state_screen::screen_LEVEL){
        listeDeButton[8].isPressed = true;
    }
    if(mouseXpos >= listeDeButton[9].posX+1 && mouseXpos < listeDeButton[9].posX+listeDeButton[9].width+1 && 
    mouseYpos >= listeDeButton[9].posY && mouseYpos < listeDeButton[9].posY + listeDeButton[9].height && _state == state_screen::screen_LEVEL){
        listeDeButton[9].isPressed = true;
    }

}

void App::scroll_callback(double /*xoffset*/, double /*yoffset*/) {
}

void App::cursor_position_callback(double /*xpos*/, double /*ypos*/) {
}

void App::size_callback(int width, int height)
{
    _width  = width;
    _height = height;

    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, _width, _height);

    const float aspectRatio { _width / (float) _height };

    // Change the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(aspectRatio > 1.0f) {
        glOrtho(-_viewSize / 2.0f * aspectRatio, _viewSize / 2.0f * aspectRatio, -_viewSize / 2.0f, _viewSize / 2.0f, -1.0f, 1.0f);
    } else {
        glOrtho(-_viewSize / 2.0f, _viewSize / 2.0f, -_viewSize / 2.0f / aspectRatio, _viewSize / 2.0f / aspectRatio, -1.0f, 1.0f);
    }
}