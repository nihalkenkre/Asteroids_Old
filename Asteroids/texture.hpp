#pragma once

#include <tiny_gltf.h>
#include <vulkan/vulkan.hpp>

class texture
{
public:
    texture ();
    texture (const tinygltf::Image& image);
    texture (const texture& other);
    texture& operator= (const texture& other);
    texture (texture&& other) noexcept;
    texture& operator= (texture&& other) noexcept;
    ~texture () noexcept;

    std::string name;

    std::vector<unsigned char> image_data;
    vk::DeviceSize image_data_offset;

    vk::DeviceSize width;
    vk::DeviceSize height;

    vk::Image image;
    vk::ImageView image_view;
};