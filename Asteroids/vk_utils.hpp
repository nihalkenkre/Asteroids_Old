#pragma once

#include <vulkan/vulkan.hpp>

class vk_utils
{
public:
    static vk::UniqueBuffer create_buffer (vk::DeviceSize size, vk::BufferUsageFlags usage);
    static vk::UniqueDeviceMemory create_memory_for_buffer (vk::UniqueBuffer buffer, vk::MemoryPropertyFlags required_memory_types);

private:
    static uint32_t get_memory_type_index (vk::MemoryRequirements memory_requirements, vk::MemoryPropertyFlags required_memory_types);
};