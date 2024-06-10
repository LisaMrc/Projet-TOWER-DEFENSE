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
    // load what needs to be loaded here (for example textures)
    img::Image test {img::load(make_absolute_path("images/level.png", true), 3, true)};
    
    // LAND TEXTURES
    img::Image grass {img::load(make_absolute_path("images/textures/land/grass.bmp", true), 3, true)};
    img::Image path {img::load(make_absolute_path("images/textures/land/path.bmp", true), 3, true)};
    
    // ENTITIES TEXTURES
    img::Image king {img::load(make_absolute_path("images/textures/entities/king.png", true), 3, true)};
    img::Image knight {img::load(make_absolute_path("images/textures/entities/knight.png", true), 3, true)};
    img::Image tower {img::load(make_absolute_path("images/textures/entities/tower.png", true), 3, true)};

    _texture = loadTexture(test);

    // _grass = loadTexture(grass);
    // _path = loadTexture(path);

    // _king = loadTexture(king);
    // _knight = loadTexture(knight);
    // _tower = loadTexture(tower);
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

    // Map printing
    std::unordered_map<glm::vec3, CaseType> RGB_CaseType_map = associate_RGB_to_CaseType(splitted_itd_file);
    associate_px_pos_to_CaseType(RGB_CaseType_map);

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

/*
void App::render() {
    // Clear the color and depth buffers of the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // glBegin(GL_QUADS);

    // glVertex2f(0, 0);

    // glVertex2f(0, 0.5);

    // glVertex2f(0.5, 0.5);

    // glVertex2f(0.5, 0);

    // glEnd();

    draw_grid();

    TextRenderer.Render();
}
*/

float x = 1;
float y = 1;
Map map;

void App::render()
{
    // Clear the color and depth buffers of the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw the grid
    draw_grid();

    // Draw the quad with texture
    draw_quad_with_texture(_texture, x, y, map);
    // draw_map();

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