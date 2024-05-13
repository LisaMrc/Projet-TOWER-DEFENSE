#include "screens.hpp"

void quadrillage()
{
    for (size_t i = 0; i < count; i++)
    {
        for (size_t i = 0; i < count; i++)
        {
            glColor2f(1, 0, 0);
            glBegin(GL_LINES);
                glVertex2f(0, 10);
                glVertex2f(0, -10);
            glEnd();
        }
        
    }
}