#pragma once

#include <vector>


class static_graphics_primitive
{
public:
    static_graphics_primitive ();
    ~static_graphics_primitive ();

    std::vector <unsigned char> positions;
};

class static_graphics_primitives
{
public:
    static_graphics_primitives ();
    ~static_graphics_primitives ();

    std::vector<static_graphics_primitive> primitives;
};

class static_physics_primitive
{
public:
    static_physics_primitive ();
    ~static_physics_primitive ();

    std::vector <unsigned char> positions;
};

class static_physics_primitives
{
public:
    static_physics_primitives ();
    ~static_physics_primitives ();

    std::vector<static_physics_primitive> primitives;
};
