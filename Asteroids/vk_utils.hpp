#pragma once

#include <Windows.h>
#include <vulkan/vulkan.hpp>
#include <map>


class vk_instance
{
public:
    vk_instance ();
    vk_instance (const bool& is_validation_needed);
    vk_instance (const vk_instance& other) = delete;
    vk_instance& operator= (const vk_instance& other) = delete;
    vk_instance (vk_instance&& other) noexcept;
    vk_instance& operator= (vk_instance&& other) noexcept;
    ~vk_instance () noexcept;

    vk::Instance instance;
};


class vk_debug_utils_messenger
{
public:
    vk_debug_utils_messenger ();
    vk_debug_utils_messenger (const vk::Instance& instance);
    vk_debug_utils_messenger (const vk_debug_utils_messenger& other) = delete;
    vk_debug_utils_messenger& operator= (const vk_debug_utils_messenger& other) = delete;
    vk_debug_utils_messenger (vk_debug_utils_messenger&& other) noexcept;
    vk_debug_utils_messenger& operator= (vk_debug_utils_messenger&& other) noexcept;
    ~vk_debug_utils_messenger () noexcept;

    vk::DebugUtilsMessengerEXT debug_utils_messenger;

private:
    vk::Instance instance;
};


class vk_physical_device
{
public:
    vk_physical_device (const vk::Instance& instance);

    vk::PhysicalDevice physical_device;
    vk::PhysicalDeviceMemoryProperties memory_properties;
    vk::PhysicalDeviceLimits limits;
};


class vk_queue_family_indices
{
public:
    vk_queue_family_indices (const vk::PhysicalDevice& physical_device);

    uint32_t graphics_queue_family_index;
    uint32_t compute_queue_family_index;
    uint32_t transfer_queue_family_index;
};


class vk_surface
{
public:
    vk_surface ();
    vk_surface (const vk::Instance& instance, const HINSTANCE& h_instance, const HWND& h_wnd, const vk::PhysicalDevice& physical_device, const uint32_t& graphics_queue_family_index);
    vk_surface (const vk_surface& other) = delete;
    vk_surface& operator= (const vk_surface& other) = delete;
    vk_surface (vk_surface&& other) noexcept;
    vk_surface& operator= (vk_surface&& other) noexcept;
    ~vk_surface () noexcept;

    vk::SurfaceKHR surface;
    vk::PresentModeKHR present_mode;
    vk::SurfaceCapabilitiesKHR surface_capabilities;
    vk::SurfaceFormatKHR surface_format;
    vk::Extent2D surface_extent;

private:
    vk::Instance instance;
};


class vk_queue_info
{
public:
    vk_queue_info (const vk_queue_family_indices* queue_family_indices);

    std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
    std::vector<size_t> queue_indices;
};

class vk_graphics_device
{
public:
    vk_graphics_device ();
    vk_graphics_device (const vk::PhysicalDevice& physical_device, const std::vector<vk::DeviceQueueCreateInfo>& queue_create_infos);
    vk_graphics_device (const vk_graphics_device& other) = delete;
    vk_graphics_device& operator= (const vk_graphics_device& other) = delete;
    vk_graphics_device (vk_graphics_device&& other) noexcept;
    vk_graphics_device& operator= (vk_graphics_device&& other) noexcept;
    ~vk_graphics_device () noexcept;

    vk::Device graphics_device;
};


class vk_device_queues
{
public:
    vk_device_queues (const vk::PhysicalDevice& physical_device, const vk::Device& graphics_device, const vk_queue_family_indices* queue_family_indices, const std::vector<uint32_t>& queue_indices);

    vk::Queue graphics_queue;
    vk::Queue compute_queue;
    vk::Queue transfer_queue;
};


class vk_image_view
{
public:
    vk_image_view ();
    vk_image_view (const vk::Device& graphics_device, const vk::Image& image, const vk::Format& format);
    vk_image_view (const vk_image_view& other) = delete;
    vk_image_view& operator= (const vk_image_view& other) = delete;
    vk_image_view (vk_image_view&& other) noexcept;
    vk_image_view& operator= (vk_image_view&& other) noexcept;
    ~vk_image_view () noexcept;

    vk::ImageView image_view;

private:
    vk::Device graphics_device;
};


class vk_swapchain
{
public:
    vk_swapchain ();
    vk_swapchain (const vk::Device& graphics_device, const vk_surface* surface);
    vk_swapchain (const vk_swapchain& other) = delete;
    vk_swapchain& operator= (const vk_swapchain& other) = delete;
    vk_swapchain (vk_swapchain&& other) noexcept;
    vk_swapchain& operator= (vk_swapchain&& other) noexcept;
    ~vk_swapchain () noexcept;

    vk::SwapchainKHR swapchain;
    std::vector<vk::Image> swapchain_images;
    std::vector<vk_image_view> swapchain_image_views;

private:
    vk::Device graphics_device;
};


class vk_command_pool
{
public:
    vk_command_pool ();
    vk_command_pool (const vk::Device& graphics_device, const size_t& queue_family_index, const vk::CommandPoolCreateFlags& flags);
    vk_command_pool (const vk_command_pool& other) = delete;
    vk_command_pool& operator= (const vk_command_pool& other) = delete;
    vk_command_pool (vk_command_pool&& other) noexcept;
    vk_command_pool& operator=(vk_command_pool&& other) noexcept;
    ~vk_command_pool () noexcept;

    vk::CommandPool command_pool;
    
private:
    vk::Device graphics_device;
};


class vk_command_buffer
{
public:
    vk_command_buffer ();
    vk_command_buffer (const vk::Device graphics_device, const vk::CommandPool& command_pool);
    vk_command_buffer (const vk_command_buffer& other) = delete;
    vk_command_buffer& operator= (const vk_command_buffer& other) = delete;
    vk_command_buffer (vk_command_buffer&& other) noexcept;
    vk_command_buffer& operator= (vk_command_buffer&& other) noexcept;
    ~vk_command_buffer () noexcept;

    void begin ();
    void end ();

    vk::CommandBuffer command_buffer;

private:
    vk::CommandPool command_pool;
    vk::Device graphics_device;
};


class vk_image
{
public:
    vk_image ();
    vk_image (const vk::Device& graphics_device, const uint32_t& queue_family_index, const vk::Extent3D& extent, const uint32_t& array_layers, const vk::Format& format, const vk::ImageLayout& image_layout, vk::SharingMode sharing_mode, vk::ImageUsageFlags usage);
    vk_image (const vk_image& other) = delete;
    vk_image& operator= (const vk_image& other) = delete;
    vk_image (vk_image&& other) noexcept;
    vk_image& operator= (vk_image&& other) noexcept;
    ~vk_image () noexcept;

    void change_layout (const vk::Queue& transfer_queue, const vk::CommandPool& transfer_command_pool, const uint32_t& src_queue_family_index, const uint32_t& dst_queue_family_index, const vk::Image& image, const uint32_t& num_layers, const vk::ImageLayout& old_layout, const vk::ImageLayout& new_layout, const vk::AccessFlags& src_access, const vk::AccessFlags& dst_access, const vk::PipelineStageFlags& src_pipeline_stage, const vk::PipelineStageFlags& dst_pipeline_stage);
    void copy_from () {}
    void copy_to () {}

    vk::Image image;

private:
    vk::Device graphics_device;
};


class vk_buffer
{
public:
    vk_buffer ();
    vk_buffer (const vk::Device& graphics_device, const vk::DeviceSize& size, const vk::BufferUsageFlags& usage, const vk::SharingMode& sharing_mode, const uint32_t& graphics_queue_family_index);
    vk_buffer (const vk_buffer& other) = delete;
    vk_buffer& operator= (const vk_buffer& other) = delete;
    vk_buffer (vk_buffer&& other) noexcept;
    vk_buffer& operator= (vk_buffer&& other) noexcept;
    ~vk_buffer () noexcept;

    void bind_memory (const vk::DeviceMemory& memory, const vk::DeviceSize& offset);
    void copy_from (const vk::Buffer& src_buffer, const vk::DeviceSize& size, const vk::CommandPool& command_pool, const vk::Queue& transfer_queue);
    void copy_to () {}

    vk::Buffer buffer;

private:
    vk::Device graphics_device;
};


class vk_device_memory
{
public:
    vk_device_memory ();
    vk_device_memory (const vk::Device& graphics_device, const vk::Buffer& buffer, const vk::PhysicalDeviceMemoryProperties& memory_properties, vk::MemoryPropertyFlags required_types);
    vk_device_memory (const vk::Device& graphics_device, const vk::ArrayProxy<vk::Image>& images, const vk::PhysicalDeviceMemoryProperties& memory_properties, vk::MemoryPropertyFlags required_types);
    vk_device_memory (const vk::Device& graphics_device, const vk::ArrayProxy<vk_image*>& images, const vk::PhysicalDeviceMemoryProperties& memory_properties, vk::MemoryPropertyFlags required_types);
    vk_device_memory (const vk_device_memory& other) = delete;
    vk_device_memory& operator= (const vk_device_memory& other) = delete;
    vk_device_memory (vk_device_memory&& other) noexcept;
    vk_device_memory& operator= (vk_device_memory&& other) noexcept;
    ~vk_device_memory () noexcept;

    void bind_buffer (const vk::Buffer& buffer, const vk::DeviceSize& offset);
    void bind_images (const std::map<vk::Image, vk::DeviceSize>& images_offsets);
    void map_data (const std::vector<unsigned char>& data, const vk::DeviceSize& offset);

    vk::DeviceMemory device_memory;

private:
    vk::Device graphics_device;
};


class vk_queue
{
public:
    vk_queue (const vk::Queue& queue, const vk::Device& graphics_device);

    void submit (const vk::ArrayProxy<vk::CommandBuffer>& command_buffers);

private:
    vk::Device graphics_device;
    vk::Queue queue;
};