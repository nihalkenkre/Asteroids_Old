#pragma once

#include <tiny_gltf.h>

#include "texture.hpp"

#undef OPAQUE // disable wingdi background definition enum - we are not using it at all
enum class MATERIAL_ALPHA_MODE
{
    OPAQUE,
    MASK,
    BLEND,
};

class material
{
public:
    material ();
    material (const tinygltf::Material& material);
    material (const material& other);
    material& operator= (const material& other);
    material (material&& other) noexcept;
    material& operator= (material&& other) noexcept;
    ~material () noexcept;

    std::string name;

    texture base_texture;
    texture metalness_roughness_texture;
    texture normal_texture;
    texture occlusion_texture;
    texture emissive_texture;

    std::array<float, 4> base_color_factor;
    float metalness_factor;
    float roughness_factor;
    std::array<float, 3> emissive_factor;

    MATERIAL_ALPHA_MODE alpha_mode;
};