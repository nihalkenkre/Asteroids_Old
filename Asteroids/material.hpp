#pragma once

#include <tiny_gltf.h>
#include <memory>

#include "image.hpp"


#undef OPAQUE // disable wingdi background definition enum - we are not using it at all
enum class MATERIAL_ALPHA_MODE
{
    OPAQUE,
    ALPHA,
    BLEND,
};

class material
{
public:
    material (const tinygltf::Material& material, const tinygltf::Model& model);

    std::string name;

    std::unique_ptr<image> base_image;
    std::array<float, 4> base_color_factor;

    MATERIAL_ALPHA_MODE alpha_mode;
};