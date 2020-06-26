#pragma once

#include <vector>

#include "static_mesh.hpp"
#include "material.hpp"
#include "image.hpp"

#include "gltf_models.hpp"

class scene_assets
{
public:
    scene_assets (const gltf_models& models, const std::vector<std::string>& search_names);

    std::vector<static_mesh> static_meshes;
    std::vector<material> materials;
    std::vector<image> images;
};