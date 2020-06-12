#pragma once

#include <Windows.h>
#include <memory>
#include <vulkan/vulkan.hpp>

class vk_instance;
class vk_debug_utils_messenger;
class vk_surface;
class vk_graphics_device;
class vk_swapchain;
class vk_command_pool;
class vk_image_view;


class common_graphics
{
public:
    common_graphics (HINSTANCE h_instance, HWND h_wnd);
    ~common_graphics ();

    std::unique_ptr<vk_instance> instance = nullptr;
    std::unique_ptr<vk_graphics_device> graphics_device = nullptr;
    std::unique_ptr<vk_debug_utils_messenger> debug_utils_messenger = nullptr;
    std::unique_ptr<vk_surface> surface = nullptr;
    std::unique_ptr<vk_swapchain> swapchain = nullptr;
    std::vector<vk::Image> swapchain_images;
    std::vector<std::unique_ptr<vk_image_view>> swapchain_image_views;
    std::unique_ptr<vk_command_pool> transfer_command_pool = nullptr;

    vk::PhysicalDeviceMemoryProperties physical_device_memory_properties;
    vk::PhysicalDeviceLimits physical_device_limits;
    vk::SurfaceFormatKHR surface_format;
    vk::Extent2D surface_extent;
    size_t num_swapchain_images;
    vk::Queue graphics_queue;
    vk::Queue transfer_queue;
    vk::Queue compute_queue;

    size_t graphics_queue_family_index;
    size_t transfer_queue_family_index;
    size_t compute_queue_family_index;

private:
    void get_physical_device ();
    void get_queue_properties ();
    void get_surface_properties ();
    std::pair <std::vector<vk::DeviceQueueCreateInfo>, std::vector<size_t>> get_queue_data ();
    void get_device_queues (const std::vector<size_t>& queue_indices);
    void create_transfer_command_pool ();

    bool is_validation_needed = false;

    vk::PhysicalDevice physical_device;
    vk::PresentModeKHR present_mode;
    vk::SurfaceCapabilitiesKHR surface_capabilities;
};