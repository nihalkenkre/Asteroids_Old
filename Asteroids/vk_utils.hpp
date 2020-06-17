#pragma once

#include <Windows.h>
#include <vulkan/vulkan.hpp>


class vk_instance
{
public:
    vk_instance ();
    vk_instance (const vk_instance& other);
    vk_instance& operator= (const vk_instance& other);
    vk_instance (vk_instance&& other) noexcept;
    vk_instance& operator= (vk_instance&& other) noexcept;
    vk_instance (const bool& is_validation_needed);
    ~vk_instance () noexcept;

    inline vk::Instance get_obj ()
    {
        return instance;
    }

private:
    vk::Instance instance;
};


class vk_debug_utils_messenger
{
public:
    vk_debug_utils_messenger ();
    vk_debug_utils_messenger (const vk_debug_utils_messenger& other);
    vk_debug_utils_messenger& operator= (const vk_debug_utils_messenger& other);
    vk_debug_utils_messenger (vk_debug_utils_messenger&& other) noexcept;
    vk_debug_utils_messenger& operator= (vk_debug_utils_messenger&& other) noexcept;
    vk_debug_utils_messenger (const vk::Instance& instance);
    ~vk_debug_utils_messenger () noexcept;

    inline vk::DebugUtilsMessengerEXT get_obj ()
    {
        return debug_utils_messenger;
    }

private:
    vk::DebugUtilsMessengerEXT debug_utils_messenger;
    vk::Instance instance;
};


class vk_surface
{
public:
    vk_surface (const vk::Instance& instance, HINSTANCE h_instance, HWND h_wnd);
    ~vk_surface ();

    inline vk::SurfaceKHR get_obj ()
    {
        return surface;
    }

private:
    vk::SurfaceKHR surface;
    vk::Instance instance;
};


class vk_graphics_device
{
public:
    vk_graphics_device (const vk::PhysicalDevice& physical_device, const std::vector<vk::DeviceQueueCreateInfo>& queue_create_infos);
    ~vk_graphics_device ();

    inline vk::Device get_obj ()
    {
        return graphics_device;
    }

private:
    vk::Device graphics_device;
};


class vk_swapchain
{
public:
    vk_swapchain (const vk::Device& graphics_device, const vk::SurfaceKHR& surface, const vk::SurfaceCapabilitiesKHR& surface_capabilities, const vk::SurfaceFormatKHR& surface_format, const vk::Extent2D& surface_extent, const vk::PresentModeKHR& present_mode);
    ~vk_swapchain ();

    inline vk::SwapchainKHR get_obj ()
    {
        return swapchain;
    }

private:
    vk::SwapchainKHR swapchain;
    vk::Device graphics_device;
};


class vk_command_pool
{
public:
    vk_command_pool ();
    vk_command_pool (const vk_command_pool& other);
    vk_command_pool& operator= (const vk_command_pool& other);
    vk_command_pool (vk_command_pool&& other) noexcept;
    vk_command_pool& operator=(vk_command_pool&& other) noexcept;
    vk_command_pool (const vk::Device& graphics_device, const size_t& queue_family_index, const vk::CommandPoolCreateFlags& flags);
    ~vk_command_pool () noexcept;

    inline vk::CommandPool get_obj ()
    {
        return command_pool;
    }

private:
    vk::CommandPool command_pool;
    vk::Device graphics_device;
};


class vk_image_view
{
public:
    vk_image_view (const vk::Device& graphics_device, const vk::Image& image, const vk::Format& format);
    ~vk_image_view ();

    inline vk::ImageView get_obj ()
    {
        return image_view;
    }

private:
    vk::ImageView image_view;
    vk::Device graphics_device;
};

/*class vk_utils
{
public:
    static vk::Buffer create_buffer (vk::DeviceSize size, vk::BufferUsageFlags usage);
    static vk::DeviceMemory create_memory_for_buffer (vk::Buffer buffer, vk::MemoryPropertyFlags required_memory_types);
    static void map_data_to_device_memory (vk::DeviceMemory& memory, vk::DeviceSize offset, vk::DeviceSize size, void* data_source);
    static void copy_buffer_to_buffer (vk::Buffer& src_buffer, vk::Buffer& dst_buffer, vk::DeviceSize size);

    static vk::DeviceMemory create_memory_for_images (const std::vector<vk::Image>& images, vk::MemoryPropertyFlags required_memory_types);
    static void change_image_layout (vk::Image& image, uint32_t num_layers, uint32_t src_family_queue_index, uint32_t dst_family_queue_index, vk::ImageLayout old_layout, vk::ImageLayout new_layout, vk::AccessFlags src_access, vk::AccessFlags dst_access, vk::PipelineStageFlags src_stage, vk::PipelineStageFlags dst_stage);
    static void copy_buffer_to_image (vk::Buffer buffer, vk::Image& image, vk::DeviceSize offset, vk::Extent3D extent);
    static vk::ImageView create_image_view (vk::Image& image);
    
    static vk::CommandPool create_command_pool (size_t queue_family_index, vk::CommandPoolCreateFlags flags);
    static void destroy_command_pool_and_buffers (vk::CommandPool command_pool);

    static void destroy_buffer_and_memory (vk::Buffer buffer, vk::DeviceMemory buffer_memory);

private:
    static uint32_t get_memory_type_index (vk::MemoryRequirements memory_requirements, vk::MemoryPropertyFlags required_memory_types);
    static vk::CommandBuffer get_one_time_command_buffer (vk::CommandPool command_pool);
    static void submit_one_time_command_buffer (vk::Queue queue, vk::CommandBuffer command_buffer);
};*/