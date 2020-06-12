#pragma once

#include <Windows.h>
#include <memory>
#include <vulkan/vulkan.hpp>

class vk_instance;
class vk_debug_utils_messenger;
class vk_surface;

class common_graphics
{
public:
    common_graphics (HINSTANCE h_instance, HWND h_wnd);
    ~common_graphics ();
    void init (HINSTANCE h_instance, HWND h_wnd);

    static vk::Device graphics_device;
    static vk::PhysicalDeviceMemoryProperties physical_device_memory_properties;
    static vk::PhysicalDeviceLimits physical_device_limits;
    static vk::SurfaceFormatKHR surface_format;
    static vk::Extent2D surface_extent;
    static vk::SwapchainKHR swapchain;
    static std::vector<vk::Image> swapchain_images;
    static std::vector<vk::ImageView> swapchain_image_views;
    static size_t num_swapchain_images;
    static vk::Queue graphics_queue;
    static vk::Queue transfer_queue;
    static vk::Queue compute_queue;
    static vk::CommandPool transfer_command_pool;

    static size_t graphics_queue_family_index;
    static size_t transfer_queue_family_index;
    static size_t compute_queue_family_index;

private:
    //void create_instance ();
    //void setup_debug_utils_messenger ();
    //void create_surface (HINSTANCE h_instance, HWND h_wnd);
    void get_physical_device ();
    void get_surface_properties ();
    void create_graphics_device ();
    void get_device_queues ();
    void create_swapchain ();
    void create_transfer_command_pool ();

    bool is_validation_needed = false;

    vk::PhysicalDevice physical_device;
    vk::PresentModeKHR present_mode;
    vk::SurfaceCapabilitiesKHR surface_capabilities;
    std::unique_ptr<vk_instance> instance = nullptr;
    std::unique_ptr<vk_debug_utils_messenger> debug_utils_messenger = nullptr;
    std::unique_ptr<vk_surface> surface = nullptr;
};