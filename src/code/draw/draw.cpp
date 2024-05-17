#include "../../App.hpp"
#include "../ui/shape.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>

#include <sstream>

#include "simpletext.h"
#include "utils.hpp"
#include "GLHelpers.hpp"
#include <iostream>

// GDN : we have decided to grid the square screen by a 8x8 grid. The integer "u" corresponds to the number of units on the rows and columns

float const u = 8;

void grid()
{
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    for (float i = -1; i < 1; i+=1-1/u)
    {
        glVertex2f(-1, i);
        glVertex2f(1, i);
        glVertex2f(i, -1);
        glVertex2f(i, 1);
    }
    glEnd();
}

// std::vector<float> Case::convert_coord(std::vector<int> abs_coord)
// {

// }