#pragma once

#include "vk_utils.hpp"

#include <tiny_gltf.h>
#include <vulkan/vulkan.hpp>

class image
{
public:
    image (const tinygltf::Image& image, const tinygltf::Model& model);

    std::string name;

    std::vector<unsigned char> image_data;

    uint32_t width;
    uint32_t height;

    vk::DeviceSize image_data_offset;

    vk_image img;
    vk_image_view img_view;
};
