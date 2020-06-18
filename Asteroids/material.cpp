#include "material.hpp"

material::material ()
{
}

material::material (const tinygltf::Material& material)
{
}

material::material (const material& other)
{
}

material& material::operator=(const material& other)
{
    return *this;
}

material::material (material&& other) noexcept
{
}

material& material::operator=(material&& other) noexcept
{
    return *this;
}

material::~material () noexcept
{
}
