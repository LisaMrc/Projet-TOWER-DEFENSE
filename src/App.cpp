#include <iostream>
#include "App.hpp"
#include "code/draw/draw.hpp"
#include "code/entities/entities.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>

#include "simpletext.h"
#include "utils.hpp"
#include "GLHelpers.hpp"

App::App() : _previousTime(0.0), _viewSize(2.0) {
    img::Image test {img::load(make_absolute_path("images/level.png", true), 4, true)};
    map._texture = loadTexture(test);
    
    // LAND TEXTURES
    img::Image grass {img::load(make_absolute_path("images/textures/land/grass.bmp", true), 4, true)};
    img::Image path {img::load(make_absolute_path("images/textures/land/path.bmp", true), 4, true)};
    img::Image in {img::load(make_absolute_path("images/textures/land/in.png", true), 4, true)};
    img::Image out {img::load(make_absolute_path("images/textures/land/out.png", true), 4, true)};
    
    // ENTITIES TEXTURES
    img::Image king {img::load(make_absolute_path("images/textures/entities/king.png", true), 4, true)};
    img::Image knight {img::load(make_absolute_path("images/textures/entities/knight.png", true), 4, true)};
    img::Image tower {img::load(make_absolute_path("images/textures/entities/tower.png", true), 4, true)};

    map._grass = loadTexture(grass);
    map._path = loadTexture(path);
    map._in = loadTexture(in);
    map._out = loadTexture(out);

    kinger._king = loadTexture(king);

    Purrsival._knight = loadTexture(knight);
}

void App::setup()
{
    // Set the clear color to a nice blue
    glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

    // Setup the text renderer with blending enabled and white text color
    TextRenderer.ResetFont();
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::WHITE);
    TextRenderer.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    TextRenderer.EnableBlending(true);

    // Extract information from itd file
    std::vector<std::vector<std::string>> splitted_itd_file = split_itd_file();

    // Tools to print map
    map.associate_RGB_to_CaseType(splitted_itd_file);
    map.associate_px_pos_to_CaseType();

    // Create graph for ennemies from itd
    std::vector<std::vector<float>> adjacency_matrix {create_adjacency_matrix(splitted_itd_file)};
    Graph::WeightedGraph graph {Graph::build_from_adjacency_matrix(adjacency_matrix)};
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
    const double currentTime { glfwGetTime() };
    const double elapsedTime { currentTime - _previousTime};
    _previousTime = currentTime;

    Purrsival.get_elapsedTime(elapsedTime);

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

    // Draw the grid
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
}

void App::key_callback(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/) {
}

void App::mouse_button_callback(int /*button*/, int /*action*/, int /*mods*/) {

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