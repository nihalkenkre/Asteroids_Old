#pragma once

#include <Windows.h>
#include <vulkan/vulkan.hpp>


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
    vk_debug_utils_messenger (const vk::Instance& instance);
    vk_debug_utils_messenger (const vk_debug_utils_messenger& other) = delete;
    vk_debug_utils_messenger& operator= (const vk_debug_utils_messenger& other) = delete;
    vk_debug_utils_messenger (vk_debug_utils_messenger&& other) noexcept;
    vk_debug_utils_messenger& operator= (vk_debug_utils_messenger&& other) noexcept;
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
    vk_surface ();
    vk_surface (const vk::Instance& instance, HINSTANCE h_instance, HWND h_wnd);
    vk_surface (const vk_surface& other) = delete;
    vk_surface& operator= (const vk_surface& other) = delete;
    vk_surface (vk_surface&& other) noexcept;
    vk_surface& operator= (vk_surface&& other) noexcept;
    ~vk_surface () noexcept;

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
    vk_graphics_device ();
    vk_graphics_device (const vk::PhysicalDevice& physical_device, const std::vector<vk::DeviceQueueCreateInfo>& queue_create_infos);
    vk_graphics_device (const vk_graphics_device& other) = delete;
    vk_graphics_device& operator= (const vk_graphics_device& other) = delete;
    vk_graphics_device (vk_graphics_device&& other) noexcept;
    vk_graphics_device& operator= (vk_graphics_device&& other) noexcept;
    ~vk_graphics_device () noexcept;

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
    vk_swapchain ();
    vk_swapchain (const vk::Device& graphics_device, const vk::SurfaceKHR& surface, const vk::SurfaceCapabilitiesKHR& surface_capabilities, const vk::SurfaceFormatKHR& surface_format, const vk::Extent2D& surface_extent, const vk::PresentModeKHR& present_mode);
    vk_swapchain (const vk_swapchain& other) = delete;
    vk_swapchain& operator= (const vk_swapchain& other) = delete;
    vk_swapchain (vk_swapchain&& other) noexcept;
    vk_swapchain& operator= (vk_swapchain&& other) noexcept;
    ~vk_swapchain () noexcept;

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
    vk_command_pool (const vk::Device& graphics_device, const size_t& queue_family_index, const vk::CommandPoolCreateFlags& flags);
    vk_command_pool (const vk_command_pool& other) = delete;
    vk_command_pool& operator= (const vk_command_pool& other) = delete;
    vk_command_pool (vk_command_pool&& other) noexcept;
    vk_command_pool& operator=(vk_command_pool&& other) noexcept;
    ~vk_command_pool () noexcept;

    inline vk::CommandPool get_obj ()
    {
        return command_pool;
    }

private:
    vk::CommandPool command_pool;
    vk::Device graphics_device;
};

class vk_image
{
public:
    vk_image ();
    vk_image (const vk_image& other) = delete;
    vk_image& operator= (const vk_image& other) = delete;
    vk_image (vk_image&& other) noexcept;
    vk_image& operator= (vk_image&& other) noexcept;
    ~vk_image () noexcept;

    inline vk::Image get_obj ()
    {
        return image;
    }
private:
    vk::Image image;
    vk::Device graphics_device;
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

    inline vk::ImageView get_obj ()
    {
        return image_view;
    }

private:
    vk::ImageView image_view;
    vk::Device graphics_device;
};
