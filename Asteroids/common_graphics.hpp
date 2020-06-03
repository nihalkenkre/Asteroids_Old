#pragma once

#include <Windows.h>
#include <vulkan/vulkan.hpp>

class common_graphics
{
public:
    void init (HINSTANCE h_instance, HWND h_wnd);
    void shutdown ();

    static vk::UniqueDevice graphics_device;
    static vk::PhysicalDeviceMemoryProperties physical_device_memory_properties;
    static vk::PhysicalDeviceLimits physical_device_limits;
    static vk::SurfaceFormatKHR surface_format;
    static vk::Extent2D surface_extent;
    static vk::UniqueSwapchainKHR swapchain;
    static std::vector<vk::UniqueImage> swapchain_images;
    static std::vector<vk::UniqueImageView> swapchain_image_views;;
    static size_t num_swapchain_images;
    static vk::Queue graphics_queue;
    static vk::Queue transfer_queue;
    static vk::Queue compute_queue;
    static vk::UniqueCommandPool transfer_command_pool;

    static size_t graphics_queue_family_index;
    static size_t transfer_queue_family_index;
    static size_t compute_queue_family_index;

private:
    void populate_instance_layers_and_extensions ();
    void create_instance ();
    void setup_debug_utils_messenger ();
    void get_physical_device ();
    void create_surface ();
    void populate_graphics_device_extensions ();
    void get_surface_properties ();
    void create_graphics_device ();
    void create_swapchain ();
    void create_swapchain_imageviews ();
    void get_device_queues ();

    bool is_validation_needed = false;

    std::vector<std::string> requested_instance_layers;
    std::vector<std::string> requested_instance_extensions;
    std::vector<std::string> requested_device_extensions;

    vk::UniqueInstance instance;
    vk::UniqueDebugUtilsMessengerEXT debug_utils_messenger;
    vk::PhysicalDevice physical_device;
    vk::UniqueSurfaceKHR surface;
    vk::PresentModeKHR present_mode;
    vk::SurfaceCapabilitiesKHR surface_capabilities;
};