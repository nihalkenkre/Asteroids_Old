#pragma once

#include <tiny_gltf.h>
#include <memory>

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
    material (const tinygltf::Material& material);

    std::string name;

    std::unique_ptr<texture> base_texture;
    std::unique_ptr<texture> metalness_roughness_texture;
    std::unique_ptr<texture> normal_texture;
    std::unique_ptr<texture> occlusion_texture;
    std::unique_ptr<texture> emissive_texture;

    std::array<float, 4> base_color_factor;
    float metalness_factor;
    float roughness_factor;
    std::array<float, 3> emissive_factor;

    MATERIAL_ALPHA_MODE alpha_mode;
};