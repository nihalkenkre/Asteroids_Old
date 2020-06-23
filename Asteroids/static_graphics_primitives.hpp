#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>
#include <tiny_gltf.h>

#include "material.hpp"
#include "static_graphics_primitive.hpp"

class static_graphics_primitives
{
public:
    static_graphics_primitives (const tinygltf::Node& graphics_node, const MATERIAL_ALPHA_MODE& material_mode, const tinygltf::Model& model);
    
    std::vector<static_graphics_primitive> primitives;
};