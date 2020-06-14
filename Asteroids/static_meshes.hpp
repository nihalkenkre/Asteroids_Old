#pragma once

#include <tiny_gltf.h>

#include <Windows.h>
#include <memory>


class static_mesh 
{
public:
    static_mesh (const tinygltf::Node & graphics_node, const std::vector<tinygltf::Node>& physics_nodes, const tinygltf::Model& model);
    ~static_mesh ();
};


#include <vector>


class gltf_models;


class static_meshes
{
public:
    static_meshes (const gltf_models& models, const std::vector<std::string>& search_names);
    ~static_meshes ();

    std::vector <static_mesh> meshes;
};