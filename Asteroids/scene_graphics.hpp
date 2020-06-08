#pragma once

#include "vk_asset.hpp"

#include <vulkan/vulkan.hpp>
#include <tiny_gltf.h>


class scene_graphics
{
public:
    scene_graphics ();
    ~scene_graphics ();

    void create_graphics_for_meshes (const std::vector<std::string>& file_paths);
    void create_graphics_for_images (const std::vector<std::string>& file_paths);

private:
    /*void import_images (const std::vector<tinygltf::Model>& models);
    void import_materials (const std::vector<tinygltf::Model>& models);*/

    void import_static_meshes (const std::vector<tinygltf::Model>& models);
    vk_static_mesh create_static_mesh (int mesh_index, const tinygltf::Model& model);
    vk_static_primitive create_static_primitive (const tinygltf::Primitive& primitive, const tinygltf::Model& model);
    vk_material create_material (int material_index, const tinygltf::Model& model);
    vk_image create_image (int material_index, const tinygltf::Model& model);

    std::vector<vk_static_mesh> static_meshes;

    vk::Buffer vertex_index_buffer;
    vk::DeviceMemory vertex_index_buffer_memory;
    
    vk::DeviceMemory images_memory;
};