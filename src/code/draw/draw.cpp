#include "../../App.hpp"
#include "../ui/shape.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>

#include "simpletext.h"
#include "utils.hpp"
#include "GLHelpers.hpp"

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

// GDN : we have decided to grid the square screen by a 8x8 grid
float const u = 4;

void grid()
{
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    for (float i = -1; i < 1 + 1/u; i+=1/u)
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

bool is_loaded_map_valid()
{
    std::ifstream myfile ("../../../data/map.itd");
    std::string myline;

    if (myfile.is_open())
    {
        char mychar;
        while ( myfile )
        {
            std::getline (myfile, myline);
            std::cout << myline << ": " << myfile.tellg() << '\n';
        }
    }
    else
    {
        //  cas 3 : fichier image existant
        std::cout << "File not found : could not be opened" << std::endl;
    }

    // cas 1 : bon ordre

    // cas 2 : triplet rgb valide

    // cas 4 : Existence d'au moins une zone d'entrée et de sortie

    // cas 5 : Existence d'au moins un chemin entre la zone d'entrée et de sortie

    std::cout << "Loaded map valid" << std::endl;
    return true;
}

// récupérer une donnée depuis une ligne:
// std::string line{"monster_count 8"};
// stringstream line_stream;
 
// // inserting string s in geek stream
// line_stream << line;
 
// // The object has the value 12345
// // and stream it to the integer x
// std::string line_name;
// int monster_count = 0;

// line_stream >> line_name;
// line_stream >> monster_count ;
 
//     // Now the variable x holds the
//     // value 12345
// cout << "Value " << line_name << " : " << monster_count;