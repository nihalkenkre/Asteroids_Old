#pragma once

#include <memory>
#include <string>

#include <vector>

class Image 
{
public:
    std::string Name;
    std::vector<unsigned char> Pixels;
};