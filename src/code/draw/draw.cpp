#include "../../App.hpp"
#include "draw.hpp"
#include "sil.hpp"
#include "utils.hpp"
#include "code/entities/entities.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <unordered_map>
#include <stack>
#include <queue>
#include <img/img.hpp>

#include <GLHelpers.hpp>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

std::vector<std::string> split_string(std::string const& s)
{
    std::istringstream in(s);
    return std::vector<std::string>(std::istream_iterator<std::string>(in), std::istream_iterator<std::string>()); 
}

// Grille de 8*8 carrés servant de repère
void draw_grid()
{
    float const u = 4;

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

void Graph::WeightedGraph::add_vertex(int id)
{
   auto it = adjacency_list.find (id);

    if (it == adjacency_list.end())
        adjacency_list[id];
    else
        std::cout << id << " is already in the graph" << std::endl;
}

void Graph::WeightedGraph::add_directed_edge(int from, int to, float weight)
{
    add_vertex(to);
    adjacency_list[from].push_back({to, weight});
}

void Graph::WeightedGraph::add_undirected_edge(int const from, int const to, float const weight)
{
    add_directed_edge(from, to, 1.0F);
    add_directed_edge(to, from, 1.0F);
}

Graph::WeightedGraph Graph::build_from_adjacency_matrix(const std::vector<std::vector<float>> &adjacency_matrix)
{
    Graph::WeightedGraph output_graph{};

    for (int x = 0; x < adjacency_matrix.size(); x++)
    {
        for (int y = 0; y < adjacency_matrix.size(); y++)
        {
            if (adjacency_matrix[x][y] != 0)
            {
                output_graph.add_directed_edge(x, y, adjacency_matrix[x][y]);
            }
        }
    }
    return output_graph;
}

std::unordered_map<int, std::pair<float, int>> Graph::WeightedGraph::dijkstra(int const &start, int const end)
    {
        std::unordered_map<int, std::pair<float, int>> distances{};
        std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<std::pair<float, int>>> to_visit{};
        std::vector<int> visited;
        to_visit.push({0.f, start});

        while (!to_visit.empty())
        {
            int current_node{to_visit.top().second};
            float current_distanceToStart{to_visit.top().first};
            to_visit.pop();

            if (current_node == end)
                return distances;

            for (auto edge : this->adjacency_list.at(current_node))
            {
                int current_adj_node{edge.to};
                float current_adj_distanceToParent{edge.weight};
                if (std::find(visited.begin(), visited.end(), current_adj_node) == visited.end())
                {
                    float adj_distanceToStart = current_distanceToStart + current_adj_distanceToParent;
                    if (distances.find(current_adj_node) == distances.end())
                        distances.insert({current_adj_node, {adj_distanceToStart, current_node}});
                    else if (distances.at(current_adj_node).first > adj_distanceToStart)
                        distances[current_adj_node] = {adj_distanceToStart, current_node};
                    to_visit.push({adj_distanceToStart, current_adj_node});
                }
            }
            visited.push_back(current_node);
        }

    return distances;
}

const char * filepath {"../../data/map.itd"};

// Lit et découpe le fichier itd
std::vector<std::vector<std::string>>split_itd_file()
{
    std::ifstream map_itd (filepath);

    std::string line;
    std::vector<std::string> splitted_line;
    std::vector<std::vector<std::string>>splitted_itd_file;

    // Vérification : est-ce que le fichier existe ?
    if (map_itd)
    {   
        // Sépare chaque ligne, puis chaque mot de chaque ligne dans un vecteur, puis l'ajoute à table
        while (std::getline (map_itd, line))
        {
            splitted_line = split_string(line);
            splitted_itd_file.push_back(splitted_line);
        }
    }
    else
    {
        std::cout << "File not found : could not be opened" << std::endl;
    }

    return splitted_itd_file;
}

// Vérification : les triplets après les mots "path", "in" ou "out" sont-ils valides ? + remplissage de la map où seront associées les couleurs à un type de case (RGB_CaseType_map), selon le fichier itd
std::unordered_map<glm::vec3, CaseType> Map::associate_RGB_to_CaseType(std::vector<std::vector<std::string>> splitted_itd_file)
{
    for (std::vector<std::string> splitted_line : splitted_itd_file)
    {
        if (splitted_line[0] == "path")
        {
            this->RGB_CaseType_map[glm::vec3{stof(splitted_line[1])/255, stof(splitted_line[2])/255, stof(splitted_line[3])/255}] = CaseType::PATH; 
        }
        else if (splitted_line[0] == "in")
        {
            this->RGB_CaseType_map[glm::vec3{stof(splitted_line[1])/255, stof(splitted_line[2])/255, stof(splitted_line[3])/255}] = CaseType::IN;
        }
        else if (splitted_line[0] == "out")
        {
            this->RGB_CaseType_map[glm::vec3{stof(splitted_line[1])/255, stof(splitted_line[2])/255, stof(splitted_line[3])/255}] = CaseType::OUT;
        }
    }

    // Ajout de la couleur qui définit "l'herbe"
    RGB_CaseType_map[glm::vec3{0, 0, 0}] = CaseType::GRASS;

    return this->RGB_CaseType_map;
}

// Associe une position de pixel (x, y) à un type de Case
std::vector<CaseType> Map::associate_px_pos_to_CaseType()
{
    sil::Image map {sil::Image("images/map/map.png")};
    this->px_pos_CaseType_vec.resize(map.width() * map.height());

    for (int x = 0; x < map.width(); x++)
    {
        for (int y = 0; y < map.height(); y++)
        {
            const auto it {this->RGB_CaseType_map.find(map.pixel(x,y))};

            if (it != this->RGB_CaseType_map.end())
            {
                this->px_pos_CaseType_vec[x + map.width()*y] = (*it).second;
            } 
            else
            {
                std::cout << "Erreur : la couleur n'existe pas" << std::endl;
            }
        }
    }
    return this->px_pos_CaseType_vec;
}

std::vector<std::vector<float>> create_adjacency_matrix(const std::vector<std::vector<std::string>> splitted_itd_file)
{
    std::vector<std::vector<int>> nodes_list {};
    int number_of_nodes {};

    std::vector<float> adjacency_line {};
    std::vector<std::vector<float>> adjacency_matrix {};

    for (std::vector<std::string> vec_str : splitted_itd_file)
    {
        // Nombre de nodes
        if (vec_str[0] == "graph")
        {
            number_of_nodes = std::stoi(vec_str[1]);
        }

        // Stockage des nodes
        else if (vec_str[0] == "node")
        {
            std::vector<int> vec_tmp {};
                
            for (int j=1; j<vec_str.size(); j++)
            {
                vec_tmp.push_back(std::stoi(vec_str[j]));
            }
            
            nodes_list.push_back(vec_tmp);
        }
    }

    // Initialisation de la matrice d'adjacence
    for (int i = 0; i < number_of_nodes; i++)
    {
        adjacency_line.push_back(0);
    }

    for (int i=0; i < number_of_nodes; i++)
    {
        adjacency_matrix.push_back(adjacency_line);
    }

    // Remplissage de la matrice
    for (int i=0; i < nodes_list.size(); i++)
    {
        if (nodes_list[i].size() > 3)
        {
            for (int j = 3; j < nodes_list[i].size(); j++)
            {
                int ind = nodes_list[i][j];
                adjacency_matrix[i][ind] = 1;
            }
        }
    }

    for (int i=0; i<number_of_nodes; i++)
    {
        for (int j=0; j<number_of_nodes; j++) {
            if (adjacency_matrix[i][j] == 1) {
                adjacency_matrix[i][j] = abs(nodes_list[j][1] - nodes_list[i][1])  +  abs(nodes_list[j][2] - nodes_list[i][2]);
            }
        }
    }

    return adjacency_matrix;
}

// Dessine une case texturisée à la position (x,y)
void draw_quad_with_texture(GLuint const &texture, float &x, float &y, Map &map)
{
    float X0 = -1 + map.PIXEL_SIZE*x;
    float X1 = X0 + map.PIXEL_SIZE;
    float Y0 = 1 - y*map.PIXEL_SIZE - map.PIXEL_SIZE;
    float Y1 = Y0 + map.PIXEL_SIZE;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3ub(255, 255, 255);

    glBegin(GL_QUADS);
        glTexCoord2d(0, 0);
        glVertex2f(X0, Y0);

        glTexCoord2d(1, 0);
        glVertex2f(X1, Y0);

        glTexCoord2d(1, 1);
        glVertex2f(X1, Y1);

        glTexCoord2d(0, 1);
        glVertex2f(X0, Y1);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void draw_quad_with_texture_v2(GLuint const &texture)
{

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3ub(255, 255, 255);

    glBegin(GL_QUADS);
        glTexCoord2d(0, 0);
        glVertex2f(-0.5, -0.5);

        glTexCoord2d(1, 0);
        glVertex2f(0.5, -0.5);

        glTexCoord2d(1, 1);
        glVertex2f(0.5, 0.5);

        glTexCoord2d(0, 1);
        glVertex2f(-0.5, 0.5);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

// Dessine la map
void Map::draw_map (Map &map)
{
    sil::Image imagemap {sil::Image("images/map/map.png")};
 
    for (float x = 0; x < 8; x++)
    {
        for (float y = 0; y < 8; y++)
        {
            if (this->px_pos_CaseType_vec[x + imagemap.width()* y] == CaseType::PATH)
            {
                draw_quad_with_texture(this->_path, x, y, map);
            }
            else if (this->px_pos_CaseType_vec[x + imagemap.width()* y] == CaseType::GRASS)
            {
                draw_quad_with_texture(this->_grass, x, y, map);
            }
            else if (this->px_pos_CaseType_vec[x + imagemap.width()* y] == CaseType::IN)
            {
                draw_quad_with_texture(this->_in, x, y, map);
            }
            else if (this->px_pos_CaseType_vec[x + imagemap.width()* y] == CaseType::OUT)
            {
                draw_quad_with_texture(this->_out, x, y, map);
            }
        }
    }
}

bool Map::can_create_tower(Map &map, float x, float y)
{
    sil::Image imagemap {sil::Image("images/map/map.png")};

    if (x < 0 || y < 0 || x >= imagemap.width() || y >= imagemap.height()) 
    {
        return false;
    }

    // Calculez l'index de la position
    int index = static_cast<int>(x) + imagemap.width() * static_cast<int>(y);

    // Vérifiez le type de case
    switch (this->px_pos_CaseType_vec[index])
    {
        case CaseType::PATH:
        case CaseType::IN:
        case CaseType::OUT:
        // case CaseType::OCCUPIED:
            return false;
        case CaseType::GRASS:
            // this->px_pos_CaseType_vec[index] = CaseType::OCCUPIED;
            return true;
        default:
            return false;
    }    
}