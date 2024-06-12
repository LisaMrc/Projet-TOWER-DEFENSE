#include "../../App.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>

#include <sstream>

#include "simpletext.h"
#include "utils.hpp"
#include "GLHelpers.hpp"
#include <iostream>


// void clic_button_callback(GLFWwindow* window, int button, int action, int mods){
//     //utiliser les coordonnées openGL pas cases et placer en parametres double xpos ypos

//     if (0){//les coordonnées du curseur (xpos et ypos) correspondent a la position du bouton start alors :
//         if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
//             //lancer l'ecran du jeu
//             std::cout << "jeu demarer" ;
//         }
//     }
//     if (0){//les coordonnées du curseur (xpos et ypos) correspondent a la position du bouton quit alors :
//         if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
//             //quitter le jeu
//         }
//     }
//     if (0){//les coordonnées du curseur (xpos et ypos) correspondent a la position du bouton pause alors :
//         if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
//             //mettre le jeu en pause
//         }
//     }
// }


void Button::draw_me(){  //permet de dessiner le bouton au bon endroit
    glPushMatrix();
        glTranslatef(-0.125 - 0.75, 0.125 + 0.75, 0); //permet de partir de la case en haut a gauche ; case (0,0)
        glTranslatef(0.25*posX, -0.25*posY, 0);
        glTranslatef((width%2 == 1) ? 0 : 0.125, (height%2 == 1) ? 0 : -0.125, 0);
        glScalef(0.25, 0.25, 1); //ramene a une taille d'une case
        glScalef(width, height, 1);
        
        draw_quad_with_texture(texture);
    glPopMatrix();
}

 