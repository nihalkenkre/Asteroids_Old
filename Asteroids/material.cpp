#include "material.hpp"

material::material ()
{
}

material::material (const tinygltf::Material& material)
{
}

material::material (const material& other)
{
    OutputDebugString (L"material::material Copy constructor\n");

    *this = other;
}

material& material::operator=(const material& other)
{
    OutputDebugString (L"material::material Copy assignment\n");

    name = other.name;
    base_texture = other.base_texture;
    metalness_roughness_texture = other.metalness_roughness_texture;
    normal_texture = other.normal_texture;
    occlusion_texture = other.occlusion_texture;
    emissive_texture = other.emissive_texture;

    base_color_factor = other.base_color_factor;
    metalness_factor = other.metalness_factor;
    roughness_factor = other.roughness_factor;
    emissive_factor = other.emissive_factor;

    alpha_mode = other.alpha_mode;
    
    return *this;
}

material::material (material&& other) noexcept
{
    OutputDebugString (L"material::material Move constructor\n");

    *this = std::move (other);
}

material& material::operator=(material&& other) noexcept
{
    OutputDebugString (L"material::material Move assignment\n");

    name = other.name;
    base_texture = std::move (other.base_texture);
    metalness_roughness_texture = std::move (other.metalness_roughness_texture);
    normal_texture = std::move (other.normal_texture);
    occlusion_texture = std::move (other.occlusion_texture);
    emissive_texture = std::move (other.emissive_texture);

    base_color_factor = std::move (other.base_color_factor);
    metalness_factor = other.metalness_factor;
    roughness_factor = other.roughness_factor;
    emissive_factor = std::move (other.emissive_factor);

    alpha_mode = other.alpha_mode;

    return *this;
}

material::~material () noexcept
{
}
