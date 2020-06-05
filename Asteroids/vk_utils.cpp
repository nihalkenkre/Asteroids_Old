#include "vk_utils.hpp"

#include "common_graphics.hpp"

vk::UniqueBuffer vk_utils::create_buffer (vk::DeviceSize size, vk::BufferUsageFlags usage, size_t queue_family_index)
{
    vk::BufferCreateInfo create_info ({});

    return common_graphics::graphics_device->createBufferUnique (create_info);
}
