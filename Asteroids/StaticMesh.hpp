#pragma once

#include <string>
#include <cstdint>
#include <vector>

class StaticMesh 
{
public:
    std::string Name;

    std::vector<uint8_t> Positions;
    std::vector<uint8_t> UV0s;
    std::vector<uint8_t> Indices;
    
    size_t NumIndices;
};