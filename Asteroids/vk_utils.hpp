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

class vk_queue;

class vk_device_queues
{
public:
    vk_device_queues (const vk::PhysicalDevice& physical_device, const vk::Device& graphics_device, const vk_queue_family_indices* queue_family_indices, const std::vector<uint32_t>& queue_indices);

    std::unique_ptr<vk_queue> graphics_queue = nullptr;
    std::unique_ptr<vk_queue> compute_queue = nullptr;
    std::unique_ptr<vk_queue> transfer_queue = nullptr;
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
    std::vector<vk::Image> images;
    std::vector<vk_image_view> image_views;

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

class vk_framebuffer;

class vk_command_buffers
{
public:
    vk_command_buffers ();
    vk_command_buffers (const vk::Device graphics_device, const vk::CommandPool& command_pool, const uint32_t& num_command_buffers);
    vk_command_buffers (const vk_command_buffers& other) = delete;
    vk_command_buffers& operator= (const vk_command_buffers& other) = delete;
    vk_command_buffers (vk_command_buffers&& other) noexcept;
    vk_command_buffers& operator= (vk_command_buffers&& other) noexcept;
    ~vk_command_buffers () noexcept;

    void begin (const vk::CommandBufferUsageFlags& flags);
    void draw (const vk::RenderPass& render_pass, const std::vector<vk_framebuffer>& framebuffers, const vk::Rect2D& render_area);
    void end ();

    std::vector<vk::CommandBuffer> command_buffers;

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
    void copy_from_buffer (const vk::Queue & transfer_queue, const vk::CommandPool& transfer_command_pool, const vk::DeviceSize& offset, const vk::Buffer& buffer, const vk::Extent3D& extent, const uint32_t& num_layers);
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
    void copy_from_buffer (const vk::Buffer& src_buffer, const vk::DeviceSize& size, const vk::CommandPool& command_pool, const vk::Queue& transfer_queue);
    void copy_to () {}

    vk::Buffer buffer;

private:
    vk::Device graphics_device;
};

class image;

class vk_device_memory
{
public:
    vk_device_memory ();
    vk_device_memory (const vk::Device& graphics_device, const vk::Buffer& buffer, const vk::PhysicalDeviceMemoryProperties& memory_properties, vk::MemoryPropertyFlags required_types);
    vk_device_memory (const vk::Device& graphics_device, const vk::ArrayProxy<vk::Image>& images, const vk::PhysicalDeviceMemoryProperties& memory_properties, vk::MemoryPropertyFlags required_types);
    vk_device_memory (const vk::Device& graphics_device, const std::vector<image*>& images, const vk::PhysicalDeviceMemoryProperties& memory_properties, vk::MemoryPropertyFlags required_types);
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

class vk_command_buffers;
class vk_semaphores;

class vk_queue
{
public:
    vk_queue (const vk::Queue& queue, const vk::Device& graphics_device);

    void submit (const std::vector<vk::CommandBuffer>& command_buffers) const;
    void submit (const vk::PipelineStageFlags& wait_stage_flags, const vk::CommandBuffer& command_buffer, const vk::Semaphore& wait_semaphore, const vk::Semaphore& signal_semphore) const;
    void present (const vk::SwapchainKHR& swapchain, const uint32_t& image_index, const vk::Semaphore& wait_semaphore) const;

    vk::Queue queue;

private:
    vk::Device graphics_device;
};


class vk_descriptor_pool
{
public:
    vk_descriptor_pool ();
    vk_descriptor_pool (const std::vector<std::pair<vk::DescriptorType, size_t>>& descriptor_types_counts, const uint32_t& max_sets);
    vk_descriptor_pool (const vk_descriptor_pool& other) = delete;
    vk_descriptor_pool& operator= (const vk_descriptor_pool& other) = delete;
    vk_descriptor_pool (vk_descriptor_pool&& other) noexcept;
    vk_descriptor_pool& operator= (vk_descriptor_pool&& other) noexcept;
    ~vk_descriptor_pool () noexcept;

    vk::DescriptorPool descriptor_pool;

private:
    vk::Device graphics_device;
};


class vk_descriptor_set_layout
{
public:
    vk_descriptor_set_layout ();
    vk_descriptor_set_layout (const std::vector<vk::DescriptorSetLayoutBinding>& bindings);
    vk_descriptor_set_layout (const vk_descriptor_set_layout& other) = delete;
    vk_descriptor_set_layout& operator= (const vk_descriptor_set_layout& other) = delete;
    vk_descriptor_set_layout (vk_descriptor_set_layout&& other) noexcept;
    vk_descriptor_set_layout& operator= (vk_descriptor_set_layout&& other) noexcept;
    ~vk_descriptor_set_layout () noexcept;

    vk::DescriptorSetLayout descriptor_set_layout;

private:
    vk::Device graphics_device;
};


class vk_descriptor_sets
{
public:
    vk_descriptor_sets ();
    vk_descriptor_sets (const vk::DescriptorPool& descriptor_pool, const std::vector<vk::DescriptorSetLayout>& set_layouts, const uint32_t& num_sets);
    vk_descriptor_sets (const vk_descriptor_sets& other) = delete;
    vk_descriptor_sets& operator= (const vk_descriptor_sets& other) = delete;
    vk_descriptor_sets (vk_descriptor_sets&& other) noexcept;
    vk_descriptor_sets& operator= (vk_descriptor_sets&& other) noexcept;
    ~vk_descriptor_sets () noexcept;

    std::vector<vk::DescriptorSet> descriptor_sets;

private:
    vk::DescriptorPool descriptor_pool;
    vk::Device graphics_device;
};


class vk_render_pass
{
public:
    vk_render_pass ();
    vk_render_pass (const vk::Device& graphics_device, const vk::Format& chosen_format);
    vk_render_pass (const vk_render_pass& other) = delete;
    vk_render_pass& operator= (const vk_render_pass& other) = delete;
    vk_render_pass (vk_render_pass&& other) noexcept;
    vk_render_pass& operator= (vk_render_pass&& other) noexcept;
    ~vk_render_pass () noexcept;

    vk::RenderPass render_pass;

private:
    vk::Device graphics_device;
};


class vk_framebuffer
{
public:
    vk_framebuffer ();
    vk_framebuffer (const vk::Device& graphics_device, const vk::ImageView& image_view, const vk::RenderPass& render_pass, const vk::Extent2D& extent);
    vk_framebuffer (const vk_framebuffer& other) = delete;
    vk_framebuffer& operator= (const vk_framebuffer& other) = delete;
    vk_framebuffer (vk_framebuffer&& other) noexcept;
    vk_framebuffer& operator= (vk_framebuffer&& other) noexcept;
    ~vk_framebuffer () noexcept;

    vk::Framebuffer framebuffer;

private:
    vk::Device graphics_device;
};


class vk_framebuffers
{
public:
    vk_framebuffers (const vk::Device& graphics_device, const std::vector<vk_image_view>& image_views, const vk::RenderPass& render_pass, const vk::Extent2D& extent);

    std::vector<vk_framebuffer> framebuffers;
};

class vk_graphics_pipeline
{
public:
    vk_graphics_pipeline ();
    vk_graphics_pipeline (const vk_graphics_pipeline& other) = delete;
    vk_graphics_pipeline& operator= (const vk_graphics_pipeline& other) = delete;
    vk_graphics_pipeline (vk_graphics_pipeline&& other) noexcept;
    vk_graphics_pipeline& operator= (vk_graphics_pipeline&& other) noexcept;
    ~vk_graphics_pipeline () noexcept;

    vk::Pipeline graphics_pipeline;

private:
    vk::Device graphics_device;
};


class vk_semaphore
{
public:
    vk_semaphore ();
    vk_semaphore (const vk::Device& graphics_device);
    vk_semaphore (const vk_semaphore& other) = delete;
    vk_semaphore& operator= (const vk_semaphore& other) = delete;
    vk_semaphore (vk_semaphore&& other) noexcept;
    vk_semaphore& operator= (vk_semaphore&& other) noexcept;
    ~vk_semaphore () noexcept;

    vk::Semaphore semaphore;

private:
    vk::Device graphics_device;
};


class vk_semaphores
{
public:
    vk_semaphores (const vk::Device& graphics_device, const size_t& num_semaphores);

    std::vector<vk_semaphore> semaphores;
};