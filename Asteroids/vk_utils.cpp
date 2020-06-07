#include "vk_utils.hpp"

#include "common_graphics.hpp"

vk::UniqueBuffer vk_utils::create_buffer (vk::DeviceSize size, vk::BufferUsageFlags usage)
{
    vk::BufferCreateInfo create_info ({}, size, usage, vk::SharingMode::eExclusive);

    return common_graphics::graphics_device->createBufferUnique (create_info);
}

vk::UniqueDeviceMemory vk_utils::create_memory_for_buffer (vk::Buffer buffer, vk::MemoryPropertyFlags required_memory_types)
{
    vk::MemoryRequirements memory_requirements = common_graphics::graphics_device->getBufferMemoryRequirements (buffer);
    vk::MemoryAllocateInfo allocate_info (memory_requirements.size, get_memory_type_index (memory_requirements, required_memory_types));
    vk::UniqueDeviceMemory out_device_memory = common_graphics::graphics_device->allocateMemoryUnique (allocate_info);
    common_graphics::graphics_device->bindBufferMemory (buffer, out_device_memory.get (), 0);

    return out_device_memory;
}

void vk_utils::map_data_to_device_memory (vk::DeviceMemory memory, vk::DeviceSize offset, vk::DeviceSize size, void* data_source)
{
    void* data = common_graphics::graphics_device->mapMemory (memory, offset, size);
    std::memcpy (data, data_source, (size_t)size);
    common_graphics::graphics_device->unmapMemory (memory);
}

vk::UniqueDeviceMemory vk_utils::create_memory_for_images (const std::vector<vk::UniqueImage>& images, vk::MemoryPropertyFlags required_memory_types)
{
    vk::MemoryAllocateInfo allocate_info = {};

    std::vector<vk::DeviceSize> offsets;
    offsets.reserve (images.size ());

    for (const auto& image : images)
    {
        offsets.push_back (allocate_info.allocationSize);
        vk::MemoryRequirements memory_requirements = common_graphics::graphics_device->getImageMemoryRequirements (image.get ());
        allocate_info.allocationSize += memory_requirements.size;
        allocate_info.memoryTypeIndex = get_memory_type_index (memory_requirements, required_memory_types);
    }

    vk::UniqueDeviceMemory out_device_memory = common_graphics::graphics_device->allocateMemoryUnique (allocate_info);

    size_t current_index = 0;
    for (const auto& image : images)
    {
        common_graphics::graphics_device->bindImageMemory (image.get (), out_device_memory.get (), offsets.at (current_index));
        ++current_index;
    }

    return out_device_memory;
}

void vk_utils::change_image_layout (vk::Image image, uint32_t num_layers, uint32_t src_family_queue_index, uint32_t dst_family_queue_index, vk::ImageLayout old_layout, vk::ImageLayout new_layout, vk::AccessFlags src_access, vk::AccessFlags dst_access, vk::PipelineStageFlags src_stage, vk::PipelineStageFlags dst_stage)
{
    vk::UniqueCommandBuffer one_time_command_buffer = get_one_time_command_buffer (common_graphics::transfer_command_pool.get ());

    vk::ImageSubresourceRange subresource_range (vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
    vk::ImageMemoryBarrier image_memory_barrier (src_access, dst_access, old_layout, new_layout, common_graphics::transfer_queue_family_index, common_graphics::transfer_queue_family_index, image, subresource_range);

    one_time_command_buffer->pipelineBarrier (src_stage, dst_stage, vk::DependencyFlagBits::eDeviceGroup, 0, 0, image_memory_barrier);
    one_time_command_buffer->end ();

    submit_one_time_command_buffer (common_graphics::transfer_queue, one_time_command_buffer.get ());
    common_graphics::graphics_device->freeCommandBuffers (common_graphics::transfer_command_pool.get (), one_time_command_buffer.get ());
}

vk::UniqueCommandPool vk_utils::create_command_pool (size_t queue_family_index, vk::CommandPoolCreateFlags flags)
{
    vk::CommandPoolCreateInfo create_info (flags, queue_family_index);
    return common_graphics::graphics_device->createCommandPoolUnique (create_info);
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

vk::UniqueCommandBuffer vk_utils::get_one_time_command_buffer (vk::CommandPool command_pool)
{
    vk::CommandBufferAllocateInfo allocate_info (command_pool, vk::CommandBufferLevel::ePrimary, 1);
    vk::UniqueCommandBuffer out_command_buffer = std::move (common_graphics::graphics_device->allocateCommandBuffersUnique (allocate_info).at (0));

    vk::CommandBufferBeginInfo begin_info (vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
    out_command_buffer->begin (begin_info);

    return out_command_buffer;
}

void vk_utils::submit_one_time_command_buffer (vk::Queue queue, vk::CommandBuffer command_buffer)
{
    vk::SubmitInfo submit_info ({}, {}, {}, 1, &command_buffer);
    queue.submit (submit_info, 0);

    queue.waitIdle ();
}
