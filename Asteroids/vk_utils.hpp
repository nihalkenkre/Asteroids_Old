#pragma once

#include <vulkan/vulkan.hpp>

class vk_utils
{
public:
    static vk::UniqueBuffer create_buffer (vk::DeviceSize size, vk::BufferUsageFlags usage, size_t queue_family_index);
};