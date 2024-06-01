#include "../../App.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>

#include <sstream>

#include "simpletext.h"
#include "utils.hpp"
#include "GLHelpers.hpp"
#include <iostream>


void clic_button_callback(GLFWwindow* window, int button, int action, int mods){
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
	GLint windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);		
	xpos = (xpos-windowWidth/2.f)/windowWidth; //convertion coordonnées de notre repèe dans le repère d'openGL donc point (0,0) au centre
	ypos = (ypos-windowHeight/2.f)/windowWidth;

    std::cout << "x:" << xpos;
    std::cout << "y:" << ypos;

    if (xpos>-0.5 && xpos<0.5  && ypos>-0.25 && ypos<0 ){//les coordonnées du curseur (xpos et ypos) correspondent a la position du bouton start alors :
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            //lancer l'ecran du jeu
        }
    }
    if (xpos>-0.5 && xpos<0.5 && ypos>-0.25 && ypos<0){//les coordonnées du curseur (xpos et ypos) correspondent a la position du bouton quit alors :
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            //quitter le jeu
        }
    }
    if (xpos>-0.5 && xpos<0.5 && ypos>-0.75&& ypos<-0.5){//les coordonnées du curseur (xpos et ypos) correspondent a la position du bouton pause alors :
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            //mettre le jeu en pause
        }
    }
}


 