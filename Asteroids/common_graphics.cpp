#include "common_graphics.hpp"
#include <vulkan/vulkan_win32.h>


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
    void* /*pUserData*/)
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
    instance = vk::createInstanceUnique (vk::InstanceCreateInfo ());
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
}

void common_graphics::create_surface ()
{
}

void common_graphics::populate_graphics_device_extensions ()
{
}

void common_graphics::get_surface_properties ()
{
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
#endif // _DEBUG

    populate_instance_layers_and_extensions ();
    create_instance ();

}

void common_graphics::shutdown ()
{
}
