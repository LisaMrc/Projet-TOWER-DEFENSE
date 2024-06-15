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

App::App() : _previousTime(0.0), _viewSize(2.0) {
    img::Image test {img::load(make_absolute_path("images/level.png", true), 4, true)};
    //listeDeButton[0].texture = loadTexture(test);
    
    // LAND TEXTURES
    img::Image grass {img::load(make_absolute_path("images/textures/land/grass.bmp", true), 4, true)};
    img::Image path {img::load(make_absolute_path("images/textures/land/path.bmp", true), 4, true)};
    img::Image in {img::load(make_absolute_path("images/textures/land/in.png", true), 4, true)};
    img::Image out {img::load(make_absolute_path("images/textures/land/out.png", true), 4, true)};
    
    // ENTITIES TEXTURES
    img::Image king {img::load(make_absolute_path("images/textures/entities/king.png", true), 4, true)};
    img::Image knight {img::load(make_absolute_path("images/textures/entities/knight.png", true), 4, true)};
    img::Image tower {img::load(make_absolute_path("images/textures/entities/tower.png", true), 4, true)};

    // BUTTONS TEXTURE
    img::Image start {img::load(make_absolute_path("images/button/start_button.png", true), 3, true)};
    img::Image stop {img::load(make_absolute_path("images/button/stop_button.png", true), 3, true)};
    img::Image pause {img::load(make_absolute_path("images/button/pause_button.png", true), 3, true)};

    map._grass = loadTexture(grass);
    map._path = loadTexture(path);
    map._in = loadTexture(in);
    map._out = loadTexture(out);

    kinger._king = loadTexture(king);
    Purrsival._knight = loadTexture(knight);

    arrow._arrow = loadTexture(tower);

    _texture = loadTexture(test);
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

    // Initialises the buttons
    listeDeButton.push_back(Button{"Boutton_Start", false, 3, 4, 2, 1, _texture}); //0
    listeDeButton.push_back(Button{"Boutton_Quit", false, 3, 6, 2, 1, _texture}); //1
    listeDeButton.push_back(Button{"Boutton_Pause", false, 8, 0, 1, 1, _texture}); //2
    listeDeButton.push_back(Button{"Boutton_Titre", false, 3, 1, 6, 2, _texture}); //3
    listeDeButton.push_back(Button{"Boutton_Loose", false, 3, 2, 6, 2, _texture}); //4
    listeDeButton.push_back(Button{"Boutton_Win", false, 3, 2, 6, 2, _texture}); //5
    listeDeButton.push_back(Button{"Boutton_Play_again", false, 3, 5, 2, 1, _texture}); //6

    // Extract information from itd file
    std::vector<std::vector<std::string>> splitted_itd_file = split_itd_file();

    // Tools to print map
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

    // Initialise la position du roi (il ne bougera pas)
    kinger.x = enemy_path.back().node_x;
    kinger.y = enemy_path.back().node_y;

    // Initialise la position de l'ennemi 1 (Purrsival) et on lui donne le chemin à suivre
    Purrsival.x = enemy_path.front().node_x;
    Purrsival.y = enemy_path.front().node_y;
    Purrsival.enemy_path = enemy_path;
}

void App::update()
{
    
    // std::vector<int> posCaseMouse = passage_pixel_to_case(mouseXpos, mouseYpos);
    // mouseXpos, mouseYpos = posCaseMouse[0], posCaseMouse[1];

    const double currentTime { glfwGetTime() };
    const double elapsedTime { currentTime - _previousTime};
    _previousTime = currentTime;

    // BUTTON TRIGGERS
    if(listeDeButton[0].isPressed)
    {
        _state = state_screen::screen_LEVEL;
        time_open_window = {glfwGetTime()};
    }

    if(listeDeButton[1].isPressed)
    {
        window_close = true;
    }
    
    if(listeDeButton[2].isPressed)
    {
        _state = state_screen::MENU;
    }

    // KING

    if (kinger.health == 0)
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
    if(listeDeButton[6].isPressed){
        _state = state_screen::MENU;        
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

    if(_state == state_screen::screen_LEVEL){
        time_play = glfwGetTime();
        listeDeButton[0].isPressed = false;

        // Draw a helpful grid
        // draw_grid();

        // Draw the map
        map.draw_map(map);

        // Draw the King
        draw_quad_with_texture(kinger._king, kinger.x, kinger.y, map);

        // Draw the first knight
        if (Purrsival.target_node_id < Purrsival.enemy_path.size())
        {
            Purrsival.enemy_move();
            // draw quad ici si ennemi doit disparaître
        }
        draw_quad_with_texture(Purrsival._knight, Purrsival.x, Purrsival.y, map);

        
        for (const auto& tower : towers) {
            create_tower(map, arrow, tower.x, tower.y);
        }
        
        listeDeButton[2].draw_me();

        if (kinger.health == 0){
            _state = state_screen::screen_LOOSE;
        }

        else if (time_play > 60+time_open_window && kinger.health != 0){
            _state = state_screen::screen_WIN;
        }

        // Renders player_gold_text
        std::string GOLD_Label{" GOLD : " + std::to_string(kinger.player_gold) + " "};
        this->player_gold_text.Label(GOLD_Label.c_str() , _width / 80, 100, SimpleText::LEFT);
        this->player_gold_text.Render();
    }

    if(_state == state_screen::MENU){
        listeDeButton[0].isPressed = false;
        listeDeButton[1].isPressed = false;
        listeDeButton[2].isPressed = false;
        listeDeButton[6].isPressed = false;
        listeDeButton[0].draw_me();
        listeDeButton[1].draw_me();
        listeDeButton[3].draw_me();
    }

    if (_state == state_screen::screen_LOOSE){
        listeDeButton[4].draw_me();
        listeDeButton[6].draw_me();
        std::cout<< "vous avez perdue";
    }

    if (_state == state_screen::screen_WIN){
        listeDeButton[5].draw_me();
        listeDeButton[6].draw_me();
        std::cout<< "vous avez gagner";
    }
    


    
}

void App::key_callback(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/) {
}

// void App::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
//      if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
//         double xpos, ypos; // Coordonnées en pixels
//         glfwGetCursorPos(window, &xpos, &ypos);
//         int windowWidth, windowHeight;
//         glfwGetWindowSize(window, &windowWidth, &windowHeight);
        
//         int offset = (windowWidth - windowHeight) / 2; // Décalage sur les côtés

//         // Normaliser les coordonnées (0 à 1)
//         xpos = (xpos - offset) / windowHeight; 
//         ypos = ypos / windowHeight;

//         // Calculer les coordonnées des cases (0 à 7)
//         int xCase = static_cast<int>(xpos * 8);
//         int yCase = static_cast<int>(ypos * 8);

//         std::cout << "xCase : " << xCase << "  ";
//         std::cout << "yCase : " << yCase << std::endl;
//      }
// }


void App::mouse_button_callback(int button, int action, int mods) {
    if(mouseXpos >= listeDeButton[0].posX && mouseXpos < listeDeButton[0].posX+listeDeButton[0].width && 
    mouseYpos >= listeDeButton[0].posY && mouseYpos < listeDeButton[0].posY + listeDeButton[0].height){
        listeDeButton[0].isPressed = true;
    }
    if(mouseXpos >= listeDeButton[1].posX && mouseXpos < listeDeButton[1].posX+listeDeButton[1].width && 
    mouseYpos >= listeDeButton[1].posY && mouseYpos < listeDeButton[1].posY + listeDeButton[1].height){
        listeDeButton[1].isPressed = true;
    }
    if(mouseXpos >= listeDeButton[2].posX && mouseXpos < listeDeButton[2].posX+listeDeButton[2].width && 
    mouseYpos >= listeDeButton[2].posY && mouseYpos < listeDeButton[2].posY + listeDeButton[2].height){
        listeDeButton[2].isPressed = true;
    }
    if(mouseXpos >= listeDeButton[6].posX && mouseXpos < listeDeButton[6].posX+listeDeButton[6].width && 
    mouseYpos >= listeDeButton[6].posY && mouseYpos < listeDeButton[6].posY + listeDeButton[6].height){
        listeDeButton[6].isPressed = true;
    }
    

}

void App::scroll_callback(double /*xoffset*/, double /*yoffset*/) {
}

void App::cursor_position_callback(double /*xpos*/, double /*ypos*/) {
}

void App::size_callback(int width, int height) {
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