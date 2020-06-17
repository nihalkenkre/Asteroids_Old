#pragma once

#include <vector>

class static_graphics_primitive
{
public:
    static_graphics_primitive ();
    static_graphics_primitive (const static_graphics_primitive& other);
    static_graphics_primitive& operator= (const static_graphics_primitive& other);
    static_graphics_primitive (const static_graphics_primitive&& other) noexcept;
    static_graphics_primitive& operator= (const static_graphics_primitive&& other) noexcept;
    ~static_graphics_primitive () noexcept;

    std::vector<unsigned char> positions;
};


class static_graphics_primitives
{
public:
    static_graphics_primitives ();
    static_graphics_primitives (const static_graphics_primitives& other);
    static_graphics_primitives& operator= (const static_graphics_primitives& other);
    static_graphics_primitives (const static_graphics_primitives&& other) noexcept;
    static_graphics_primitives& operator = (const static_graphics_primitives&& other) noexcept;
    ~static_graphics_primitives () noexcept;

    std::vector<static_graphics_primitive> primitives;
};