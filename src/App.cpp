#include <iostream>
#include "App.hpp"
#include "code/draw/draw.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>

#include "simpletext.h"
#include "utils.hpp"
#include "GLHelpers.hpp"

App::App() : _previousTime(0.0), _viewSize(2.0) {
    img::Image test {img::load(make_absolute_path("images/level.png", true), 3, true)};
    map._texture = loadTexture(test);
    
    // LAND TEXTURES
    img::Image grass {img::load(make_absolute_path("images/textures/land/grass.bmp", true), 3, true)};
    img::Image path {img::load(make_absolute_path("images/textures/land/path.bmp", true), 3, true)};
    img::Image in {img::load(make_absolute_path("images/textures/land/in.png", true), 3, true)};
    img::Image out {img::load(make_absolute_path("images/textures/land/out.png", true), 3, true)};
    
    // ENTITIES TEXTURES
    img::Image king {img::load(make_absolute_path("images/textures/entities/king.png", true), 3, true)};
    img::Image knight {img::load(make_absolute_path("images/textures/entities/knight.png", true), 3, true)};
    img::Image tower {img::load(make_absolute_path("images/textures/entities/tower.png", true), 3, true)};

    map._grass = loadTexture(grass);
    map._path = loadTexture(path);
    map._in = loadTexture(in);
    map._out = loadTexture(out);

    map._king = loadTexture(king);
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

    // Verify if itd file is valid, extract information from it
    std::vector<std::vector<std::string>> splitted_itd_file = split_itd_file();

    // Tools to print map
    map.associate_RGB_to_CaseType(splitted_itd_file);
    map.associate_px_pos_to_CaseType();

    // Create graph for ennemies from itd
    std::vector<std::vector<float>> adjacency_matrix {create_adjacency_matrix(splitted_itd_file)};
    Graph::WeightedGraph graph {Graph::build_from_adjacency_matrix(adjacency_matrix)};
    std::unordered_map<int, std::pair<float, int>> d_g = graph.dijkstra(0, 7); 
}

void App::update()
{
    const double currentTime { glfwGetTime() };
    const double elapsedTime { currentTime - _previousTime};
    _previousTime = currentTime;
    
    render();
}

void App::render()
{
    // Clear the color and depth buffers of the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw the grid
    draw_grid();

    // Draw the map
    map.draw_map(map);

    // Render the text
    TextRenderer.Render();
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