#pragma once

#include <vector>

class static_graphics_primitive
{
public:
    static_graphics_primitive ();
    ~static_graphics_primitive ();

    std::vector<unsigned char> positions;
};