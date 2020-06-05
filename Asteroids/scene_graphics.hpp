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
    void import_images (const std::vector<tinygltf::Model>& models);

    vk::UniqueBuffer vertex_index_buffer;
    vk::UniqueDeviceMemory vertex_index_buffer_memory;

    std::vector<vk_image> images;
};