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

    vk::DeviceSize width;
    vk::DeviceSize height;

    vk::DeviceSize image_data_offset;

    vk_image img;
    vk_image_view img_view;
};
