#pragma once

#include <string>
#include <vector>
#include <memory>
#include <tiny_gltf.h>

#include "static_graphics_primitives.hpp"
#include "static_mesh.hpp"

class gltf_models;


class static_meshes
{
public:
    static_meshes (const gltf_models& models, const std::vector<std::string>& search_names);
    
    std::vector <static_mesh> meshes;
};