#include "common_graphics.hpp"
#include "vk_utils.hpp"

#include <cstring>
#include <map>
#include <vulkan/vulkan_win32.h>

/*PFN_vkCreateDebugUtilsMessengerEXT  pfnVkCreateDebugUtilsMessengerEXT;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT (VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pMessenger)
{
    return pfnVkCreateDebugUtilsMessengerEXT (instance, pCreateInfo, pAllocator, pMessenger);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT (VkInstance instance,
    VkDebugUtilsMessengerEXT      messenger,
    VkAllocationCallbacks const* pAllocator)
{
    return pfnVkDestroyDebugUtilsMessengerEXT (instance, messenger, pAllocator);
}

VKAPI_ATTR VkBool32 VKAPI_CALL debug_messenger_callback (VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT              message_types,
    VkDebugUtilsMessengerCallbackDataEXT const* callback_data,
    void* user_data)
{
    if (callback_data)
    {
        wchar_t buff[2048];
        swprintf_s (buff, 2048, L"Debug Messenger Callback: %hs\n", callback_data->pMessage);
        OutputDebugString (buff);
    }

    return VK_FALSE;
}*/

/*void common_graphics::create_instance ()
{
    std::vector<const char*> requested_instance_layers;
    std::vector<const char*> requested_instance_extensions;
    requested_instance_extensions.reserve (3);

    std::vector<vk::LayerProperties> layer_properties = vk::enumerateInstanceLayerProperties ();

    auto layer_iter = std::find_if (layer_properties.begin (), layer_properties.end (), [&](const vk::LayerProperties& layer_property) { return (strcmp (layer_property.layerName, "VK_LAYER_KHRONOS_validation") == 0); });
    if (layer_iter != layer_properties.end ())
    {
        requested_instance_layers.push_back (layer_iter->layerName);
    }

    std::vector<vk::ExtensionProperties> extension_properties = vk::enumerateInstanceExtensionProperties ();
    auto extension_iter = std::find_if (extension_properties.begin (), extension_properties.end (), [&](const vk::ExtensionProperties& extension_property) { return (strcmp (extension_property.extensionName, VK_KHR_SURFACE_EXTENSION_NAME) == 0); });
    if (extension_iter != extension_properties.end ())
    {
        requested_instance_extensions.push_back (extension_iter->extensionName);
    }

    extension_iter = std::find_if (extension_properties.begin (), extension_properties.end (), [&](const vk::ExtensionProperties& extension_property) { return (strcmp (extension_property.extensionName, "VK_KHR_win32_surface") == 0); });
    if (extension_iter != extension_properties.end ())
    {
        requested_instance_extensions.push_back (extension_iter->extensionName);
    }

    if (is_validation_needed)
    {
        extension_iter = std::find_if (extension_properties.begin (), extension_properties.end (), [&](const vk::ExtensionProperties& extension_property) { return (strcmp (extension_property.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0); });
        if (extension_iter != extension_properties.end ())
        {
            requested_instance_extensions.push_back (extension_iter->extensionName);
        }
    }

    vk::ApplicationInfo application_info ("Asteroids", VK_MAKE_VERSION (1, 0, 0), "AGE", VK_MAKE_VERSION (1, 0, 0), VK_API_VERSION_1_2);
    vk::InstanceCreateInfo instance_create_info ({}, &application_info, requested_instance_layers.size (), requested_instance_layers.data (), requested_instance_extensions.size (), requested_instance_extensions.data ());
 
    instance = vk::createInstance (instance_create_info);
}*/

/*void common_graphics::setup_debug_utils_messenger ()
{
    pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance->get_obj ().getProcAddr ("vkCreateDebugUtilsMessengerEXT"));
    pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance->get_obj ().getProcAddr ("vkDestroyDebugUtilsMessengerEXT"));

    vk::DebugUtilsMessageSeverityFlagsEXT severity_flags (vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose);
    vk::DebugUtilsMessageTypeFlagsEXT message_type_flags (vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);

    debug_utils_messenger = instance->get_obj ().createDebugUtilsMessengerEXT (vk::DebugUtilsMessengerCreateInfoEXT ({}, severity_flags, message_type_flags, &debug_messenger_callback));
}*/

/*void common_graphics::create_surface (HINSTANCE h_instance, HWND h_wnd)
{
    vk::Win32SurfaceCreateInfoKHR surface_create_info ({}, h_instance, h_wnd);
    surface = instance->get_obj ().createWin32SurfaceKHR (surface_create_info);
}*/

/*void common_graphics::create_graphics_device ()
{
    std::vector<const char*> requested_device_extensions;
    std::vector<vk::ExtensionProperties> extension_properties = physical_device.enumerateDeviceExtensionProperties ();

    auto iter = std::find_if (extension_properties.begin (), extension_properties.end (), [&](const vk::ExtensionProperties& extension_property) { return strcmp (extension_property.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0; });
    if (iter != extension_properties.end ())
    {
        requested_device_extensions.push_back (VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }

    vk::DeviceCreateInfo device_create_info ({}, queue_create_infos.size (), queue_create_infos.data (), 0, nullptr, requested_device_extensions.size (), requested_device_extensions.data ());
    graphics_device = physical_device.createDevice (device_create_info);
}*/

/*void common_graphics::create_swapchain ()
{
    vk::SwapchainCreateInfoKHR swapchain_create_info ({}, surface->get_obj (), surface_capabilities.minImageCount + 1, surface_format.format, surface_format.colorSpace, surface_extent, 1, surface_capabilities.supportedUsageFlags, vk::SharingMode::eExclusive, 0, {}, surface_capabilities.currentTransform, vk::CompositeAlphaFlagBitsKHR::eOpaque, present_mode);
    swapchain = graphics_device->get_obj ().createSwapchainKHR (swapchain_create_info);
    swapchain_images = graphics_device->get_obj ().getSwapchainImagesKHR (swapchain);

    vk::ComponentMapping component_mapping (vk::ComponentSwizzle::eIdentity);
    vk::ImageSubresourceRange subresource_range (vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
    vk::ImageViewCreateInfo image_view_create_info ({}, {}, vk::ImageViewType::e2D, surface_format.format, component_mapping, subresource_range);

    swapchain_image_views.reserve (swapchain_images.size ());
    for (auto& image : swapchain_images)
    {
        image_view_create_info.image = image;
        swapchain_image_views.emplace_back (graphics_device->get_obj ().createImageView (image_view_create_info));
    }
}*/

/*void common_graphics::create_transfer_command_pool ()
{
    transfer_command_pool = vk_utils::create_command_pool (transfer_queue_family_index, vk::CommandPoolCreateFlagBits::eTransient);
}*/

common_graphics::common_graphics (HINSTANCE h_instance, HWND h_wnd)
{
    OutputDebugString (L"common_graphics::common_graphics\n");

#ifdef _DEBUG
    is_validation_needed = true;
#endif

    instance = std::make_unique<vk_instance> (is_validation_needed);
    
    if (is_validation_needed)
    {
        debug_utils_messenger = std::make_unique <vk_debug_utils_messenger> (instance->get_obj ());
    }

    surface = std::make_unique<vk_surface> (instance->get_obj (), h_instance, h_wnd);
    get_physical_device ();
    get_queue_properties ();
    get_surface_properties ();

    auto queue_info = get_queue_data ();
    graphics_device = std::make_unique <vk_graphics_device> (physical_device, queue_info.first);
    get_device_queues (queue_info.second);

    swapchain = std::make_unique<vk_swapchain> (graphics_device->get_obj (), surface->get_obj (), surface_capabilities, surface_format, surface_extent, present_mode);
    swapchain_images = graphics_device->get_obj ().getSwapchainImagesKHR (swapchain->get_obj ());
    swapchain_image_views.reserve (swapchain_images.size ());
    for (const auto& image : swapchain_images)
    {
        swapchain_image_views.emplace_back (std::make_unique<vk_image_view> (graphics_device->get_obj (), image, surface_format.format));
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

common_graphics::~common_graphics ()
{
    OutputDebugString (L"common_graphics::~common_graphics\n");
}