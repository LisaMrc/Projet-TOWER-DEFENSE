#include "../../App.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>

#include <sstream>

#include "simpletext.h"
#include "utils.hpp"
#include "GLHelpers.hpp"
#include <iostream>


void Button::draw_me(){  //permet de dessiner le bouton au bon endroit
    glPushMatrix();
        glTranslatef(-0.125 - 0.75, 0.125 + 0.75, 0); //permet de partir de la case en haut a gauche ; case (0,0)
        glTranslatef(0.25*posX, -0.25*posY, 0);
        glTranslatef((width%2 == 1) ? 0 : 0.125, (height%2 == 1) ? 0 : -0.125, 0);
        glScalef(0.25, 0.25, 1); //ramene a une taille d'une case
        glScalef(width, height, 1);
        
        draw_quad_with_texture_v2(texture);
    glPopMatrix();
}

 