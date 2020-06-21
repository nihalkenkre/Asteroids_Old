#include "common_graphics.hpp"
#include "vk_utils.hpp"

#include <cstring>
#include <map>
#include <vulkan/vulkan_win32.h>


common_graphics::common_graphics (HINSTANCE h_instance, HWND h_wnd)
{
    OutputDebugString (L"common_graphics::common_graphics h_instance h_wnd\n");

#ifdef _DEBUG
    is_validation_needed = true;
#endif

    instance = std::make_unique<vk_instance> (is_validation_needed);
    
    if (is_validation_needed)
    {
        debug_utils_messenger = std::make_unique<vk_debug_utils_messenger> (instance->get_obj ());
    }

    surface = std::make_unique<vk_surface> (instance->get_obj (), h_instance, h_wnd);
    get_physical_device ();
    get_queue_properties ();
    get_surface_properties ();

    auto queue_info = get_queue_data ();
    graphics_device = std::make_unique<vk_graphics_device> (physical_device, queue_info.first);
    get_device_queues (queue_info.second);

    swapchain = std::make_unique<vk_swapchain> (graphics_device->get_obj (), surface->get_obj (), surface_capabilities, surface_format, surface_extent, present_mode);
    swapchain_images = graphics_device->get_obj ().getSwapchainImagesKHR (swapchain->get_obj ());
    swapchain_image_views.reserve (swapchain_images.size ());
    for (const auto& image : swapchain_images)
    {
        swapchain_image_views.emplace_back (vk_image_view (graphics_device->get_obj (), image, surface_format.format));
    }

    transfer_command_pool = std::make_unique<vk_command_pool> (graphics_device->get_obj (), transfer_queue_family_index, vk::CommandPoolCreateFlagBits::eTransient);
}

void common_graphics::get_physical_device ()
{
    physical_device = instance->get_obj ().enumeratePhysicalDevices ().at (0);
    
    physical_device_memory_properties = physical_device.getMemoryProperties ();
    physical_device_limits = physical_device.getProperties ().limits;
}

void common_graphics::get_queue_properties ()
{
    std::vector<vk::QueueFamilyProperties> queue_family_properties = physical_device.getQueueFamilyProperties ();

    graphics_queue_family_index = std::distance (queue_family_properties.begin (), std::find_if (queue_family_properties.begin (), queue_family_properties.end (), [&](const vk::QueueFamilyProperties& family_property) { return (family_property.queueFlags & vk::QueueFlagBits::eGraphics); }));

    auto compute_family_index_iter = std::find_if (queue_family_properties.begin (), queue_family_properties.end (), [&](const vk::QueueFamilyProperties& family_property) { return (family_property.queueFlags & vk::QueueFlagBits::eCompute) && (!(family_property.queueFlags & vk::QueueFlagBits::eGraphics)); });
    if (compute_family_index_iter != queue_family_properties.end ())
    {
        compute_queue_family_index = std::distance (queue_family_properties.begin (), compute_family_index_iter);
    }
    else
    {
        compute_queue_family_index = std::distance (queue_family_properties.begin (), std::find_if (queue_family_properties.begin (), queue_family_properties.end (), [&](const vk::QueueFamilyProperties& family_property) { return (family_property.queueFlags & vk::QueueFlagBits::eCompute); }));
    }

    auto transfer_family_index_iter = std::find_if (queue_family_properties.begin (), queue_family_properties.end (), [&](const vk::QueueFamilyProperties& family_property) { return (family_property.queueFlags & vk::QueueFlagBits::eTransfer) && (!(family_property.queueFlags & vk::QueueFlagBits::eGraphics)) && (!(family_property.queueFlags & vk::QueueFlagBits::eCompute)); });
    if (transfer_family_index_iter != queue_family_properties.end ())
    {
        transfer_queue_family_index = std::distance (queue_family_properties.begin (), transfer_family_index_iter);
    }
    else
    {
        transfer_queue_family_index = std::distance (queue_family_properties.begin (), std::find_if (queue_family_properties.begin (), queue_family_properties.end (), [&](const vk::QueueFamilyProperties& family_property) { return (family_property.queueFlags & vk::QueueFlagBits::eTransfer); }));
    }
}

void common_graphics::get_surface_properties ()
{
    bool is_supported = physical_device.getSurfaceSupportKHR (graphics_queue_family_index, surface->get_obj ());

    surface_capabilities = physical_device.getSurfaceCapabilitiesKHR (surface->get_obj ());
    surface_extent = surface_capabilities.currentExtent;

    std::vector<vk::SurfaceFormatKHR> surface_formats = physical_device.getSurfaceFormatsKHR (surface->get_obj ());

    auto format_iter = std::find_if (surface_formats.begin (), surface_formats.end (), [&](vk::SurfaceFormatKHR format) { return format == vk::Format::eB8G8R8A8Unorm; });
    if (format_iter != surface_formats.end ())
    {
        surface_format = *format_iter;
    }

    std::vector<vk::PresentModeKHR> present_modes = physical_device.getSurfacePresentModesKHR (surface->get_obj ());

    auto present_mode_iter = std::find_if (present_modes.begin (), present_modes.end (), [&](const vk::PresentModeKHR& present_mode) { return present_mode == vk::PresentModeKHR::eMailbox; });
    if (present_mode_iter != present_modes.end ())
    {
        present_mode = *present_mode_iter;
    }
    else
    {
        auto present_mode_iter = std::find_if (present_modes.begin (), present_modes.end (), [&](const vk::PresentModeKHR& present_mode) { return present_mode == vk::PresentModeKHR::eFifo; });
        present_mode = *present_mode_iter;
    }
}

std::pair <std::vector<vk::DeviceQueueCreateInfo>, std::vector<size_t>> common_graphics::get_queue_data ()
{
    std::vector<size_t> family_indices = { graphics_queue_family_index, compute_queue_family_index, transfer_queue_family_index };
    std::map<size_t, size_t> family_indices_queue_count;

    for (const auto& family_index : family_indices)
    {
        family_indices_queue_count[family_index] = std::count (family_indices.begin (), family_indices.end (), family_index);
    }

    std::vector<size_t> queue_indices;

    for (const auto& family_index_family_count : family_indices_queue_count)
    {
        for (size_t i = 0; i < family_index_family_count.second; ++i)
        {
            queue_indices.push_back (i);
        }
    }

    float queue_priorities = 1.f;

    std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;
    queue_create_infos.reserve (3);
    for (const auto& family_index_queue_count : family_indices_queue_count)
    {
        queue_create_infos.emplace_back (vk::DeviceQueueCreateInfo ({}, family_index_queue_count.first, family_index_queue_count.second, &queue_priorities));
    }

    return std::make_pair (queue_create_infos, queue_indices);
}

void common_graphics::get_device_queues (const std::vector<size_t>& queue_indices)
{
    std::vector<vk::QueueFamilyProperties> queue_family_properties = physical_device.getQueueFamilyProperties ();

    graphics_queue = graphics_device->get_obj ().getQueue (graphics_queue_family_index, queue_indices.at (0) > queue_family_properties.at (graphics_queue_family_index).queueCount ? queue_family_properties.at (graphics_queue_family_index).queueCount - 1 : queue_indices.at (0));
    compute_queue = graphics_device->get_obj ().getQueue (compute_queue_family_index, queue_indices.at (1) > queue_family_properties.at (compute_queue_family_index).queueCount ? queue_family_properties.at (compute_queue_family_index).queueCount - 1 : queue_indices.at (1));
    transfer_queue = graphics_device->get_obj ().getQueue (transfer_queue_family_index, queue_indices.at (2) > queue_family_properties.at (transfer_queue_family_index).queueCount ? queue_family_properties.at (transfer_queue_family_index).queueCount - 1 : queue_indices.at (2));
}