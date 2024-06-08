#include "../../App.hpp"
#include "draw.hpp"
#include "sil.hpp"
#include "utils.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <unordered_map>
#include <stack>
#include <queue>
#include <img/img.hpp>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

std::vector<std::string> split_string(std::string const& s)
{
    std::istringstream in(s);
    return std::vector<std::string>(std::istream_iterator<std::string>(in), std::istream_iterator<std::string>()); 
}

void draw_grid()
{
    // GDN : we have decided to grid the square screen by a 8x8 grid
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

void Graph::WeightedGraph::print_DFS(int const start) const
{
    // TODO : translate comments

    std::stack<int> stack;
    int current_node{};
    std::vector<int>visited_edges{};
    stack.push(start);

    while (!(stack.empty()))
    {
        // add nodes to visited edges list
        visited_edges.push_back(stack.top());

        // Garder en mémoire la node
        current_node = stack.top();

        // retirer le dernier élt de la pile
        stack.pop();

        // trouver la node dans la adjacency_list
        auto it = adjacency_list.find (current_node);

        // trouver les nodes liées
        auto adjacencies = (*it).second;

        // Ajouter les nodes liées dans la pile
        for (int i = 0; i < adjacencies.size(); i++)
        {
            stack.push(adjacencies[i].to);
        }
    }

    for (int visited_edge : visited_edges)
    {
        std::cout << visited_edge << std::endl;
    }
}

void Graph::WeightedGraph::print_BFS(int const start) const
{
    std::queue<int> queue;
    queue.push(start);

    int current_node{};
    std::vector<int>visited_edges{};

    while (!(queue.empty()))
    {
        // Garder en mémoire la node
        current_node = queue.front();

        // ajouter la node à la liste des sommets visités
        visited_edges.push_back(current_node);

        // retirer le premier élt de la file
        queue.pop();

        // trouver la node dans la adjacency_list
        auto it = adjacency_list.find (current_node);

        // trouver les nodes liées
        auto adjacencies = (*it).second;

        // Ajouter les nodes liées dans la pile
        for (int i = 0; i < adjacencies.size(); i++)
        {
            queue.push(adjacencies[i].to);
        }
    }

    for (int visited_edge : visited_edges)
    {
        std::cout << visited_edge << std::endl;
    }
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

std::unordered_map<glm::vec3, CaseType> associate_RGB_to_CaseType(std::vector<std::vector<std::string>> splitted_itd_file)
{
    std::unordered_map<glm::vec3, CaseType> RGB_CaseType_map;

    // Vérification : les triplets après les mots "path", "in" ou "out" sont-ils valides ? + remplissage de la map où seront associées les couleurs à un type de case (RGB_CaseType_map), selon le fichier itd
    for (std::vector<std::string> splitted_line : splitted_itd_file)
    {
        if (splitted_line[0] == "path")
        {
            RGB_CaseType_map[glm::vec3{stoi(splitted_line[1]), stoi(splitted_line[2]), stoi(splitted_line[3])}] = CaseType::PATH; 
        }
        else if (splitted_line[0] == "in")
        {
            RGB_CaseType_map[glm::vec3{stoi(splitted_line[1]), stoi(splitted_line[2]), stoi(splitted_line[3])}] = CaseType::IN;
        }
        else if (splitted_line[0] == "out")
        {
            RGB_CaseType_map[glm::vec3{stoi(splitted_line[1]), stoi(splitted_line[2]), stoi(splitted_line[3])}] = CaseType::OUT;
        }
    }

    // Ajout de la couleur qui définit "l'herbe"
    RGB_CaseType_map[glm::vec3{0, 0, 0}] = CaseType::GRASS;

    return RGB_CaseType_map;
}

std::vector<CaseType> associate_px_pos_to_CaseType(const std::unordered_map<glm::vec3, CaseType> RGB_CaseType_map)
{
    sil::Image map {sil::Image("data/map.png")};
    std::vector<CaseType> px_pos_CaseType_vec;
    px_pos_CaseType_vec.resize(map.width() * map.height());

    for (int x = 0; x < map.width()-8; x++)
    {
        for (int y = 0; y < map.height()-8; y++)
        {
            const auto it {RGB_CaseType_map.find(map.pixel(x,y))};

            if (it != RGB_CaseType_map.end())
            {
                px_pos_CaseType_vec[x + map.width()*y] = (*it).second;
            } 
            else
            {
                std::cout << "Erreur : la couleur n'existe pas" << std::endl;
            }
        }
    }
    return px_pos_CaseType_vec;
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

/*
GLuint loadTexture(const img::Image& image) {
    GLuint textureId;
    
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
    
    return textureId;
}
*/

void draw_quad_with_texture(GLuint textureId) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glColor3ub(255, 255, 255);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f, -1.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f,  1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f,  1.0f);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

