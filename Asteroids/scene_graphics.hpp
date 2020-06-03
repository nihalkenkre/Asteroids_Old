#pragma once

#include <vulkan/vulkan.hpp>


class scene_graphics
{
public:
    scene_graphics ();
    ~scene_graphics ();

    void create_graphics_for_meshes (std::vector<std::string> file_paths);
    void create_graphics_for_images (std::vector<std::string> file_paths);

private:
    void import_meshes (std::vector<std::string> file_paths);
    void import_images (std::vector<std::string> file_paths);

    vk::Buffer vertex_index_buffer;
    vk::DeviceMemory vertex_index_buffer_memory;
};