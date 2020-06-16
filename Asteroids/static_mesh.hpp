#pragma once

#include <tiny_gltf.h>

#include <memory>
#include <vector>

#include "static_graphics_primitives.hpp"


class static_mesh
{
public:
    static_mesh (const static_mesh& other);
    static_mesh& operator= (const static_mesh& other); 
    static_mesh (const static_mesh&& other) noexcept;
    static_mesh& operator= (const static_mesh&& other) noexcept;
    static_mesh (const tinygltf::Node& graphics_node, const std::vector<tinygltf::Node>& physics_nodes, const tinygltf::Model& model);
    ~static_mesh () noexcept;
    
    std::string name;

    std::unique_ptr <static_graphics_primitives> opaque_graphics_primitives = nullptr;
};
