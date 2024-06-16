#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>

#include "App.hpp"
#include "code/draw/draw.hpp"
#include "code/ui/button.hpp"
#include "code/entities/entities.hpp"
#include <unordered_map>

namespace {
    App& window_as_app(GLFWwindow* window)
    {
        return *static_cast<App*>(glfwGetWindowUserPointer(window));
    }
}

// Optional: limit the frame rate
constexpr double TARGET_TIME_FOR_FRAME { 1.0 / 60.0 };

int main() {
    // Set an error callback to display glfw errors
    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "Error " << error << ": " << description << std::endl;
    });

    // Initialize glfw
    if (!glfwInit()) {
        return -1;
    }

#ifdef __APPLE__
    // We need to explicitly ask for a 3.3 context on Mac
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    // Create window
    GLFWwindow* window { glfwCreateWindow(1280, 720, "Tower Defense", nullptr, nullptr) };
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize glad (loads the OpenGL functions)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        return -1;
    }

    App app {};
    glfwSetWindowUserPointer(window, &app);

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        window_as_app(window).key_callback(key, scancode, action, mods);
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        auto& app = window_as_app(window);

        double xpos, ypos; // coordonnées en pixels
        glfwGetCursorPos(window, &xpos, &ypos);
        GLint windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        int offset = (windowWidth - windowHeight) / 2; // décalage sur les côtés

        xpos = (xpos - offset) / windowHeight; 
        ypos = ypos / windowHeight;

        // Calculer les coordonnées des cases (0 à 7)
        int xCase = static_cast<int>(xpos * 8);
        int yCase = static_cast<int>(ypos * 8);

        // std::cout << "xCase : " << xCase << "  ";
        // std::cout << "yCase : " << yCase << std::endl;

        app.xTower = static_cast<float>(xCase);
        app.yTower = static_cast<float>(yCase);

        // Vérifier si une tour existe déjà aux coordonnées spécifiées
        bool free = true;
        for (const auto& tower : app.towers_already_builds) {
            if (tower.first == app.xTower && tower.second == app.yTower) {
                free = false;
                break;
            }
        }

        // Vérifier si l'emplacement est constructible
        bool constructible = app.map.can_create_tower(app.map, app.xTower, app.yTower);

        if (free && constructible) {
            // Créer une nouvelle tour
            tower arrow{ProjectileKind::Arrow, 2, 4, app.xTower, app.yTower, 200};
            app.towers.push_back(arrow);

            // Ajouter les coordonnées de la nouvelle tour
            app.towers_already_builds.push_back(std::make_pair(app.xTower, app.yTower));

            
        } else {
            std::cout << "Il n'est pas possible de construire cette tour" << std::endl;
        }

        app.mouse_button_callback(button, action, mods);
    }
});


    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
        window_as_app(window).scroll_callback(xoffset, yoffset);
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
        window_as_app(window).cursor_position_callback(xpos, ypos);
    });
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        window_as_app(window).size_callback(width, height);
    });

    // Force calling the size_callback of the game to set the right viewport and projection matrix
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        app.size_callback(width, height);
    }

    app.setup();

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        // Enable transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Get time (in second) at loop beginning
        double startTime { glfwGetTime() };

        double xpos, ypos; // coordonnées en pixels
        glfwGetCursorPos(window, &xpos, &ypos);

        GLint windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        int offset = (windowWidth - windowHeight) / 2; // décalage sur les côtés

        xpos = ((xpos - offset) / windowHeight) * 8; 
        ypos = ((ypos) / windowHeight) * 8;

        app.mouseXpos = xpos;
        app.mouseYpos = ypos;

        app.update();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();

        // Optional: limit the frame rate
        double elapsedTime { glfwGetTime() - startTime };

        if (elapsedTime < TARGET_TIME_FOR_FRAME) {
            glfwWaitEventsTimeout(TARGET_TIME_FOR_FRAME - elapsedTime);
        }

        if (app.window_close) {
            glfwSetWindowShouldClose(window, true);
        }
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
