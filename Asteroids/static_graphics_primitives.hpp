#pragma once

#include "static_graphics_primitive.hpp"


class static_graphics_primitives
{
public:
    static_graphics_primitives ();
    ~static_graphics_primitives ();

    std::vector<static_graphics_primitive> primitives;
};