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
    std::vector<unsigned char> get_static_mesh_raw_data ();
    std::vector<vk_static_mesh> get_vk_static_meshes ();

    vk_static_primitive create_primitive (const tinygltf::Primitive& primitive, const tinygltf::Model& model);
    vk_material create_material (const tinygltf::Material& material);

    std::vector<tinygltf::Model> models;
    vk::DeviceSize current_static_mesh_data_offset;
};