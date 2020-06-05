#include "vk_utils.hpp"

#include "common_graphics.hpp"

vk::UniqueBuffer vk_utils::create_buffer (vk::DeviceSize size, vk::BufferUsageFlags usage)
{
    vk::BufferCreateInfo create_info ({}, size, usage, vk::SharingMode::eExclusive);

    return common_graphics::graphics_device->createBufferUnique (create_info);
}

vk::UniqueDeviceMemory vk_utils::create_memory_for_buffer (vk::UniqueBuffer buffer, vk::MemoryPropertyFlags required_memory_types)
{
    vk::MemoryRequirements memory_requirements = common_graphics::graphics_device->getBufferMemoryRequirements (buffer.get ());
    vk::MemoryAllocateInfo allocate_info (memory_requirements.size, get_memory_type_index (memory_requirements, required_memory_types));
    vk::UniqueDeviceMemory out_device_memory = common_graphics::graphics_device->allocateMemoryUnique (allocate_info);
    common_graphics::graphics_device->bindBufferMemory (buffer.get (), out_device_memory.get (), 0);

    return out_device_memory;
}

uint32_t vk_utils::get_memory_type_index (vk::MemoryRequirements memory_requirements, vk::MemoryPropertyFlags required_memory_types)
{
    for (uint32_t i = 0; i < common_graphics::physical_device_memory_properties.memoryTypeCount; ++i)
    {
        if (memory_requirements.memoryTypeBits & (1 << i) && required_memory_types & common_graphics::physical_device_memory_properties.memoryTypes[i].propertyFlags)
        {
            return i;
        }
    }

    return -1;
}
