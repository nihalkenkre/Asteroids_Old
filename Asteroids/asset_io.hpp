#pragma once

#include "vk_image.hpp" 
#include "vk_asset.hpp"
#include <vector>

#include <tiny_gltf.h>


class asset_io
{
public:
    asset_io (const std::vector<std::string>& file_paths);
    ~asset_io ();

    std::vector<vk_texture> get_images ();
    std::vector<vk_material> get_materials ();
    std::vector<vk_static_mesh> get_static_meshes ();

private:
    void create_images ();
    void create_materials ();
    void create_static_meshes ();
    
    std::vector<tinygltf::Model> models;
  
    std::vector<vk_texture> images;
    std::vector<vk_material> materials;
    std::vector<vk_static_mesh> static_meshes;
};