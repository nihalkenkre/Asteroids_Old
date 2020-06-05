#pragma once

#include <vulkan/vulkan.hpp>

class vk_image;

class scene_graphics
{
public:
    scene_graphics ();
    ~scene_graphics ();

    void create_graphics_for_meshes (const std::vector<std::string>& file_paths);
    void create_graphics_for_images (const std::vector<std::string>& file_paths);

private:
    vk::UniqueBuffer vertex_index_buffer;
    vk::UniqueDeviceMemory vertex_index_buffer_memory;

    std::vector<vk_image> images;
};