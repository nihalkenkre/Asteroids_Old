#pragma once


#include <string>
#include <vector>

#include "static_mesh.hpp"


class gltf_models;


class static_meshes
{
public:
    static_meshes (const gltf_models& models, const std::vector<std::string>& search_names);
    ~static_meshes ();

    std::vector <static_mesh> meshes;
};