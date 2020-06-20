#pragma once


#include <string>
#include <vector>
#include <memory>
#include <tiny_gltf.h>

#include "static_graphics_primitives.hpp"


class static_mesh
{
public:
    static_mesh ();
    static_mesh (const tinygltf::Node& graphics_node, const std::vector<tinygltf::Node>& physics_nodes, const tinygltf::Model& model);
    static_mesh (const static_mesh& other);
    static_mesh& operator= (const static_mesh& other);
    static_mesh (static_mesh&& other) noexcept;
    static_mesh& operator= (static_mesh&& other) noexcept;

    ~static_mesh () noexcept;

    std::string name;

    std::vector<static_graphics_primitive> opaque_graphics_primitives;
    std::vector<static_graphics_primitive> alpha_graphics_primitives;
    std::vector<static_graphics_primitive> blend_graphics_primitives;
};


class gltf_models;


class static_meshes
{
public:
    static_meshes ();
    static_meshes (const gltf_models& models, const std::vector<std::string>& search_names);
    static_meshes (const static_meshes& other);
    static_meshes& operator=(const static_meshes& other);
    static_meshes (static_meshes&& other) noexcept;
    static_meshes& operator = (static_meshes&& other) noexcept;
    
    ~static_meshes () noexcept;

    std::vector <static_mesh> meshes;
};