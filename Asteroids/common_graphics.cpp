#include "common_graphics.hpp"
#include <vulkan/vulkan_win32.h>

vk::PhysicalDeviceMemoryProperties common_graphics::physical_device_memory_properties;
vk::PhysicalDeviceLimits common_graphics::physical_device_limits;
vk::Extent2D common_graphics::surface_extent;
vk::SurfaceFormatKHR common_graphics::surface_format;

size_t common_graphics::graphics_queue_family_index = -1;
size_t common_graphics::transfer_queue_family_index = -1;
size_t common_graphics::compute_queue_family_index = -1;

PFN_vkCreateDebugUtilsMessengerEXT  pfnVkCreateDebugUtilsMessengerEXT;
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
}

void common_graphics::populate_instance_layers_and_extensions ()
{
    std::vector<vk::LayerProperties> layer_properties = vk::enumerateInstanceLayerProperties ();
    requested_instance_layers.reserve (1);

    auto layer_iter = std::find_if (layer_properties.begin (), layer_properties.end (), [&](vk::LayerProperties layer_property) { return (strcmp (layer_property.layerName, "VK_LAYER_LUNARG_standard_validation") == 0); });
    if (layer_iter != layer_properties.end ())
    {
        requested_instance_layers.emplace_back (layer_iter->layerName);
    }

    requested_instance_extensions.reserve (3);

    std::vector<vk::ExtensionProperties> extension_properties = vk::enumerateInstanceExtensionProperties ();
    auto extension_iter = std::find_if (extension_properties.begin (), extension_properties.end (), [&](vk::ExtensionProperties extension_property) { return (strcmp (extension_property.extensionName, VK_KHR_SURFACE_EXTENSION_NAME) == 0); });
    if (extension_iter != extension_properties.end ())
    {
        requested_instance_extensions.emplace_back (extension_iter->extensionName);
    }

    extension_iter = std::find_if (extension_properties.begin (), extension_properties.end (), [&](vk::ExtensionProperties extension_property) { return (strcmp (extension_property.extensionName, "VK_KHR_win32_surface") == 0); });
    if (extension_iter != extension_properties.end ())
    {
        requested_instance_extensions.emplace_back (extension_iter->extensionName);
    }

    if (is_validation_needed)
    {
        extension_iter = std::find_if (extension_properties.begin (), extension_properties.end (), [&](vk::ExtensionProperties extension_property) { return (strcmp (extension_property.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0); });
        if (extension_iter != extension_properties.end ())
        {
            requested_instance_extensions.emplace_back (extension_iter->extensionName);
        }
    }
}

void common_graphics::create_instance ()
{
    vk::ApplicationInfo application_info ("Asteroids", VK_MAKE_VERSION (1, 0, 0), "AGE", VK_MAKE_VERSION (1, 0, 0), VK_API_VERSION_1_2);
    vk::InstanceCreateInfo instance_create_info ({}, &application_info, requested_instance_layers.size (), requested_instance_layers.data (), requested_instance_extensions.size (), requested_instance_extensions.data ());
 
    instance = vk::createInstanceUnique (instance_create_info);
}

void common_graphics::setup_debug_utils_messenger ()
{
    pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance->getProcAddr ("vkCreateDebugUtilsMessengerEXT"));
    pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance->getProcAddr ("vkDestroyDebugUtilsMessengerEXT"));
    
    vk::DebugUtilsMessageSeverityFlagsEXT severity_flags (vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose);
    vk::DebugUtilsMessageTypeFlagsEXT message_type_flags (vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);
    
    instance->createDebugUtilsMessengerEXTUnique (vk::DebugUtilsMessengerCreateInfoEXT ({}, severity_flags, message_type_flags, &debug_messenger_callback));
}

void common_graphics::get_physical_device ()
{
    physical_device = instance->enumeratePhysicalDevices ().at (0);
    std::vector<vk::QueueFamilyProperties> queue_family_properties = physical_device.getQueueFamilyProperties ();
    
    for (size_t i = 0; i < queue_family_properties.size (); ++i)
    {
        if (queue_family_properties.at (i).queueFlags & vk::QueueFlagBits::eGraphics)
        {
            graphics_queue_family_index = i;
            break;
        }
    }

    for (size_t i = 0; i < queue_family_properties.size (); ++i)
    {
        if ((queue_family_properties.at (i).queueFlags & vk::QueueFlagBits::eCompute) && (i != graphics_queue_family_index))
        {
            compute_queue_family_index = i;
            break;
        }
    }

    if (compute_queue_family_index == -1)
	{
		for (size_t i = 0; i < queue_family_properties.size (); ++i)
		{
			if (queue_family_properties.at (i).queueFlags & vk::QueueFlagBits::eCompute)
			{
				compute_queue_family_index = i;
				break;
			}
		}	
    }

	for (size_t i = 0; i < queue_family_properties.size (); ++i)
	{
		if (queue_family_properties.at (i).queueFlags & vk::QueueFlagBits::eTransfer && (i != graphics_queue_family_index) && (i != compute_queue_family_index))
		{
			transfer_queue_family_index = i;
			break;
		}
	}

	if (transfer_queue_family_index == -1)
	{
		for (size_t i = 0; i < queue_family_properties.size (); ++i)
		{
			if (queue_family_properties.at (i).queueFlags & vk::QueueFlagBits::eTransfer)
			{
				transfer_queue_family_index = i;
				break;
			}
		}	
	}

    physical_device_memory_properties = physical_device.getMemoryProperties ();
    physical_device_limits = physical_device.getProperties ().limits;
}

void common_graphics::create_surface (HINSTANCE h_instance, HWND h_wnd)
{
    vk::Win32SurfaceCreateInfoKHR surface_create_info ({}, h_instance, h_wnd);
    instance->createWin32SurfaceKHR (surface_create_info);
}

void common_graphics::populate_graphics_device_extensions ()
{
    requested_device_extensions.reserve (1);
    std::vector<vk::ExtensionProperties> extension_properties = physical_device.enumerateDeviceExtensionProperties ();

    auto iter = std::find_if (extension_properties.begin (), extension_properties.end (), [&](vk::ExtensionProperties extension_property) { return strcmp (extension_property.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0; });
    if (iter != extension_properties.end ())
    {
        requested_device_extensions.emplace_back (VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }
}

void common_graphics::get_surface_properties ()
{
    bool is_supported = physical_device.getSurfaceSupportKHR (graphics_queue_family_index, surface.get ());

    surface_capabilities = physical_device.getSurfaceCapabilitiesKHR (surface.get ());
    surface_extent = surface_capabilities.currentExtent;

    std::vector<vk::SurfaceFormatKHR> surface_formats = physical_device.getSurfaceFormatsKHR (surface.get ());

    auto iter = std::find_if (surface_formats.begin (), surface_formats.end (), [&] (vk::SurfaceFormatKHR format) { return format == vk::Format::eB8G8R8A8Unorm; } );
    if (iter != surface_formats.end ())
    {
        surface_format = *iter;
    }

    //std::vector<vk::SurfacePresentModeKHR> present_modes = physical_device.getSurfacePresentModesKHR ();
}

void common_graphics::create_graphics_device ()
{
}

void common_graphics::create_swapchain ()
{
}

void common_graphics::create_swapchain_imageviews ()
{
}

void common_graphics::get_device_queues ()
{
}

void common_graphics::init (HINSTANCE h_instance, HWND h_wnd)
{
#ifdef _DEBUG
    is_validation_needed = true;
#endif

    populate_instance_layers_and_extensions ();
    create_instance ();

}

void common_graphics::shutdown ()
{
}
