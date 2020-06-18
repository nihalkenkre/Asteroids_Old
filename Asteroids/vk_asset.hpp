#pragma once

#include "texture.hpp"

#include <string>
#include <memory>
#include <vulkan/vulkan.hpp>


class vk_animation
{
public:
    std::string name;

    std::vector<vk::DeviceSize> frame_data_offsets;
};