#pragma once
#include <vector>

enum class CaseType
{
    GRASS,
    PATH,
    START,
    END,
};

struct Case
{
    CaseType CaseType;
    std::vector<int> coord;
    GLuint texture;

    std::vector<float> convert_coord(std::vector<int> abs_coord);
};

void grid();
bool is_loaded_map_valid();