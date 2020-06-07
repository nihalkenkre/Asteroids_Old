#pragma once

#include <vulkan/vulkan.hpp>

class vk_utils
{
public:
    static vk::UniqueBuffer create_buffer (vk::DeviceSize size, vk::BufferUsageFlags usage);
    static vk::UniqueDeviceMemory create_memory_for_buffer (vk::Buffer buffer, vk::MemoryPropertyFlags required_memory_types);
    static void map_data_to_device_memory (vk::DeviceMemory memory, vk::DeviceSize offset, vk::DeviceSize size, void* data_source);

    static vk::UniqueDeviceMemory create_memory_for_images (const std::vector<vk::UniqueImage>& images, vk::MemoryPropertyFlags required_memory_types);
    static void change_image_layout (vk::Image image, uint32_t num_layers, uint32_t src_family_queue_index, uint32_t dst_family_queue_index, vk::ImageLayout old_layout, vk::ImageLayout new_layout, vk::AccessFlags src_access, vk::AccessFlags dst_access, vk::PipelineStageFlags src_stage, vk::PipelineStageFlags dst_stage);
    static vk::UniqueCommandPool create_command_pool (size_t queue_family_index, vk::CommandPoolCreateFlags flags);

private:
    static uint32_t get_memory_type_index (vk::MemoryRequirements memory_requirements, vk::MemoryPropertyFlags required_memory_types);
    static vk::UniqueCommandBuffer get_one_time_command_buffer (vk::CommandPool command_pool);
    static void submit_one_time_command_buffer (vk::Queue queue, vk::CommandBuffer command_buffer);
};