#pragma once


#include <string>
#include <vector>
#include <memory>
#include <tiny_gltf.h>

#include "static_graphics_primitives.hpp"


class static_mesh
{
public:
    static_mesh (const tinygltf::Node& graphics_node, const std::vector<tinygltf::Node>& physics_nodes, const tinygltf::Model& model);

    std::string name;

    std::unique_ptr<static_graphics_primitives> opaque_graphics_primitives;
    std::unique_ptr<static_graphics_primitives> alpha_graphics_primitives;
    std::unique_ptr<static_graphics_primitives> blend_graphics_primitives;
};


class gltf_models;


class static_meshes
{
public:
    static_meshes (const gltf_models& models, const std::vector<std::string>& search_names);
    
    std::vector <static_mesh> meshes;
};