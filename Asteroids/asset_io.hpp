#pragma once

#include "vk_asset.hpp"
#include <vector>

#include <tiny_gltf.h>

class asset_io
{
public:
    asset_io (const std::vector<std::string>& file_paths);
    ~asset_io ();

    std::pair<std::vector<unsigned char>, std::vector<vk_static_mesh>> get_static_mesh_data ();

private:
    std::vector<tinygltf::Model> models;
};