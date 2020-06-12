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
    void bake_data_buffers ();
    
private:
    std::vector<vk_texture> images;
    std::vector<vk_material> materials;
    std::vector<vk_static_mesh> static_meshes;

    vk::Buffer vertex_index_buffer;
    vk::DeviceMemory vertex_index_buffer_memory;
    
    vk::DeviceMemory images_memory;
};