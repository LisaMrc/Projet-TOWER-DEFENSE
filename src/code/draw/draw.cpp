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
#include <vector>
#include <string>
#include <iterator>
#include <boost/lexical_cast.hpp>

#include <glm/gtx/matrix_transform_2d.hpp>
#include <sil/sil.hpp>

std::vector<std::string> split_string(std::string const& s)
{
    std::istringstream in(s); // transforme une chaîne en flux de caractères, cela simule un flux comme l'est std::cin
    // l’itérateur va lire chaque element de "in", comme un flux d'entrée, chaque élément est séparé par un espace
    return std::vector<std::string>(std::istream_iterator<std::string>(in), std::istream_iterator<std::string>()); 
}

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

void is_loaded_map_valid()
{
    std::ifstream myfile ("../../../data/map.itd");
    std::string myline;
    std::vector<std::vector<std::string>>table;

    if (myfile.is_open())
    {
        while (myfile)
        {
            std::getline (myfile, myline);
            std::vector<std::string> splitted_line = split_string(myline);
            table.push_back(splitted_line);
        }
    }
    else
    {
        //  cas 3 : fichier image existant
        std::cout << "File not found : could not be opened" << std::endl;
    }

    // cas 2 : triplet rgb valide
    for (size_t i = 0; i < table.size(); i++)
    {
        bool is_digit = 1;

        while (is_digit)
        {
            if (line[0] == "path")
            {
                int r_path = std::stoi(line[1]);
                int g_path = std::stoi(line[2]);
                int b_path = std::stoi(line[3]);
            }
            else if (line[0] == "in")
            {
                int r_in = std::stoi(line[1]);
                int g_in = std::stoi(line[2]);
                int b_in = std::stoi(line[3]);
            }
            else if (line[0] == "path")
            {
                int r_out = std::stoi(line[1]);
                int g_out = std::stoi(line[2]);
                int b_out = std::stoi(line[3]);
            }

            // is digit
        }
    }

    // cas 4 et 5 : Existence d'au moins une zone d'entrée et de sortie et Existence d'au moins un chemin entre la zone d'entrée et de sortie
    for (int i = 10; i < count; i++)
    {
        /* code : Lancer Dijkstra */
    }
    

    std::cout << "Loaded map valid" << std::endl;
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

void analyse_map(sil::Image image) 
{
    std::vector<Case>cases_in_map{};

    for (glm::vec3 & color : image.pixels()) 
    {
        Case tmp_case{};

        if (color.r = 0.f;)
        {
            /* code */
        }
    }
    image.save("output/01_onlyGreen.png");
}