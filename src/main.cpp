#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>

#include "App.hpp"

namespace {
    App& window_as_app(GLFWwindow* window)
    {
        return *static_cast<App*>(glfwGetWindowUserPointer(window));
    }
}

// Optional: limit the frame rate
constexpr double TARGET_TIME_FOR_FRAME { 1.0 / 60.0 };

int main()
{


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

    // Intialize glad (loads the OpenGL functions)
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
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) { //fonction pour convertir les coordonnées en cases
        window_as_app(window).mouse_button_callback(button, action, mods);

        double xpos, ypos; //coordonnées en pixels
        glfwGetCursorPos(window, &xpos, &ypos);
        GLint windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        int offset = (windowWidth-windowHeight)/2;	//decallage sur les côté

        //std::cout << "x:" << (xpos / windowWidth); //coordonnées openGL
        //std::cout << "y:" << (ypos / windowHeight);

        xpos = (xpos-offset)/windowHeight; 
        ypos = (ypos)/windowHeight;
       
        std::cout << "xCase : " << (int)(xpos * 8) << "  ";
        std::cout << "yCase : " << (int)(ypos * 8);
        std::cout << std::endl;
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
    while (!glfwWindowShouldClose(window))
    {
        // Enable transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Get time (in second) at loop beginning
		double startTime { glfwGetTime() };

        app.update();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();

        // Optional: limit the frame rate
		double elapsedTime { glfwGetTime() - startTime };
        // wait the remaining time to match the target wanted frame rate
		if(elapsedTime < TARGET_TIME_FOR_FRAME)
		{
			glfwWaitEventsTimeout(TARGET_TIME_FOR_FRAME-elapsedTime);
		}
    }

    glfwTerminate();
    return 0;
}
