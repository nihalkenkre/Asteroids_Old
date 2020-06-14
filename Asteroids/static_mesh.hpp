#pragma once

#include <tiny_gltf.h>

#include <Windows.h>


class static_mesh
{
public:
    static_mesh (const tinygltf::Node& node, const tinygltf::Model& model);
    ~static_mesh ();
};


#include <vector>


class gltf_models;


class static_meshes
{
public:
    static_meshes (const gltf_models& models, const std::vector<std::string>& search_names);
    ~static_meshes ();

    std::vector <std::unique_ptr<static_mesh>> meshes;
};