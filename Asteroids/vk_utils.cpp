#include "vk_utils.hpp"
#include "common_graphics.hpp"

#include <map>


vk_instance::vk_instance ()
{
    OutputDebugString (L"vk_instance::vk_instance\n");
}

vk_instance::vk_instance (const bool& is_validation_needed)
{
    OutputDebugString (L"vk_instance::vk_instance is_validation_needed\n");

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
}

vk_instance::vk_instance (vk_instance&& other) noexcept
{
    OutputDebugString (L"vk_instance::vk_instance Move constructor\n");

    *this = std::move (other);
}

vk_instance& vk_instance::operator=(vk_instance&& other) noexcept
{
    OutputDebugString (L"vk_instance::vk_instance Move assignment\n");

    instance = other.instance;
    other.instance = nullptr;

    return *this;
}

vk_instance::~vk_instance ()
{
    OutputDebugString (L"vk_instance::~vk_instance\n");

    if (instance != nullptr)
    {
        instance.destroy ();
    }
}

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

vk_debug_utils_messenger::vk_debug_utils_messenger ()
{
    OutputDebugString (L"vk_debug_utils_messenger::vk_debug_utils_messenger\n");
}

vk_debug_utils_messenger::vk_debug_utils_messenger (const vk::Instance& instance)
{
    OutputDebugString (L"vk_debug_utils_messenger::vk_debug_utils_messenger instance\n");

    pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance.getProcAddr ("vkCreateDebugUtilsMessengerEXT"));
    pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance.getProcAddr ("vkDestroyDebugUtilsMessengerEXT"));

    vk::DebugUtilsMessageSeverityFlagsEXT severity_flags (vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose);
    vk::DebugUtilsMessageTypeFlagsEXT message_type_flags (vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);

    debug_utils_messenger = instance.createDebugUtilsMessengerEXT (vk::DebugUtilsMessengerCreateInfoEXT ({}, severity_flags, message_type_flags, &debug_messenger_callback));
    this->instance = instance;
}

vk_debug_utils_messenger::vk_debug_utils_messenger (vk_debug_utils_messenger&& other) noexcept
{
    OutputDebugString (L"vk_debug_utils_messenger::vk_debug_utils_messenger Move constructor\n");

    *this = std::move (other);
}

vk_debug_utils_messenger& vk_debug_utils_messenger::operator=(vk_debug_utils_messenger&& other) noexcept
{
    OutputDebugString (L"vk_debug_utils_messenger::vk_debug_utils_messenger Move assignment\n");

    debug_utils_messenger = other.debug_utils_messenger;
    instance = other.instance;

    other.debug_utils_messenger = nullptr;
    other.instance = nullptr;

    return *this;
}

vk_debug_utils_messenger::~vk_debug_utils_messenger ()
{
    OutputDebugString (L"vk_debug_utils_messenger::~vk_debug_utils_messenger\n");

    if (instance != nullptr && debug_utils_messenger != nullptr)
    {
        instance.destroyDebugUtilsMessengerEXT (debug_utils_messenger);
    }
}

vk_surface::vk_surface ()
{
    OutputDebugString (L"vk_surface::vk_surface\n");
}

vk_surface::vk_surface (const vk::Instance& instance, const HINSTANCE& h_instance, const HWND& h_wnd, const vk::PhysicalDevice& physical_device, const uint32_t& graphics_queue_family_index)
{
    OutputDebugString (L"vk_surface::vk_surface instance h_instance h_wnd\n");

    vk::Win32SurfaceCreateInfoKHR create_info ({}, h_instance, h_wnd);
    surface = instance.createWin32SurfaceKHR (create_info);

    bool is_supported = physical_device.getSurfaceSupportKHR (graphics_queue_family_index, surface);

    surface_capabilities = physical_device.getSurfaceCapabilitiesKHR (surface);
    surface_extent = surface_capabilities.currentExtent;

    std::vector<vk::SurfaceFormatKHR> surface_formats = physical_device.getSurfaceFormatsKHR (surface);

    auto format_iter = std::find_if (surface_formats.begin (), surface_formats.end (), [&](vk::SurfaceFormatKHR format) { return format == vk::Format::eB8G8R8A8Unorm; });
    if (format_iter != surface_formats.end ())
    {
        surface_format = *format_iter;
    }

    std::vector<vk::PresentModeKHR> present_modes = physical_device.getSurfacePresentModesKHR (surface);

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

    this->instance = instance;
}

vk_surface::vk_surface (vk_surface&& other) noexcept
{
    OutputDebugString (L"vk_surface::vk_surface Move constructor\n");

    *this = std::move (other);
}

vk_surface& vk_surface::operator=(vk_surface&& other) noexcept
{
    OutputDebugString (L"vk_surface::vk_surface Move assignment\n");
    
    surface = other.surface;
    instance = other.instance;

    other.surface = nullptr;
    other.instance = nullptr;

    return *this;
}

vk_surface::~vk_surface ()
{
    OutputDebugString (L"vk_surface::~vk_surface\n");

    if (instance != nullptr && surface != nullptr)
    {
        instance.destroySurfaceKHR (surface);
    }
}

vk_graphics_device::vk_graphics_device ()
{
    OutputDebugString (L"vk_graphics_device::vk_graphics_device\n");
}

vk_graphics_device::vk_graphics_device (const vk::PhysicalDevice& physical_device, const std::vector<vk::DeviceQueueCreateInfo>& queue_create_infos)
{
    OutputDebugString (L"vk_graphics_device::vk_graphics_device physical_device queue_create_infos\n");

    std::vector<const char*> requested_device_extensions;
    std::vector<vk::ExtensionProperties> extension_properties = physical_device.enumerateDeviceExtensionProperties ();

    auto iter = std::find_if (extension_properties.begin (), extension_properties.end (), [&](const vk::ExtensionProperties& extension_property) { return strcmp (extension_property.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0; });
    if (iter != extension_properties.end ())
    {
        requested_device_extensions.push_back (VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }

    vk::DeviceCreateInfo device_create_info ({}, queue_create_infos.size (), queue_create_infos.data (), 0, nullptr, requested_device_extensions.size (), requested_device_extensions.data ());
    graphics_device = physical_device.createDevice (device_create_info);
}

vk_graphics_device::vk_graphics_device (vk_graphics_device&& other) noexcept
{
    OutputDebugString (L"vk_graphics_device::vk_graphics_device Move constructor\n");

    *this = std::move (other);
}

vk_graphics_device& vk_graphics_device::operator=(vk_graphics_device&& other) noexcept
{
    OutputDebugString (L"vk_graphics_device::vk_graphics_device Move assignment\n");

    graphics_device = other.graphics_device;

    other.graphics_device = nullptr;

    return *this;
}

vk_graphics_device::~vk_graphics_device ()
{
    OutputDebugString (L"vk_graphics_device::~vk_graphics_device\n");

    if (graphics_device != nullptr)
    {
        graphics_device.destroy ();
    }
}

vk_swapchain::vk_swapchain ()
{
    OutputDebugString (L"vk_swapchain::vk_swapchain\n");
}

vk_swapchain::vk_swapchain (const vk::Device& graphics_device, const vk_surface* surface)
{
    OutputDebugString (L"vk_swapchain::vk_swapchain graphics_device surface\n");

    vk::SwapchainCreateInfoKHR swapchain_create_info ({}, surface->surface, surface->surface_capabilities.minImageCount + 1, surface->surface_format.format, surface->surface_format.colorSpace, surface->surface_extent, 1, surface->surface_capabilities.supportedUsageFlags, vk::SharingMode::eExclusive, 0, {}, surface->surface_capabilities.currentTransform, vk::CompositeAlphaFlagBitsKHR::eOpaque, surface->present_mode);
    swapchain = graphics_device.createSwapchainKHR (swapchain_create_info);
    
    swapchain_images = graphics_device.getSwapchainImagesKHR (swapchain);
    swapchain_image_views.reserve (swapchain_images.size ());
    for (const auto& image : swapchain_images)
    {
        swapchain_image_views.emplace_back (vk_image_view (graphics_device, image, surface->surface_format.format));
    }

    this->graphics_device = graphics_device;
}

vk_swapchain::vk_swapchain (vk_swapchain&& other) noexcept
{
    OutputDebugString (L"vk_swapchain::vk_swapchain Move constructor\n");

    *this = std::move (other);
}

vk_swapchain& vk_swapchain::operator=(vk_swapchain&& other) noexcept
{
    OutputDebugString (L"vk_swapchain::vk_swapchain Move assignment\n");

    swapchain = other.swapchain;
    graphics_device = other.graphics_device;

    other.swapchain = nullptr;
    other.graphics_device = nullptr;

    return *this;
}

vk_swapchain::~vk_swapchain ()
{
    OutputDebugString (L"vk_swapchain::~vk_swapchain\n");

    if (graphics_device != nullptr && swapchain != nullptr)
    {
        graphics_device.destroySwapchainKHR (swapchain);
    }
}

vk_command_pool::vk_command_pool ()
{
    OutputDebugString (L"vk_command_pool::vk_command_pool\n");
}

vk_command_pool::vk_command_pool (const vk::Device& graphics_device, const size_t& queue_family_index, const vk::CommandPoolCreateFlags& flags)
{
    OutputDebugString (L"vk_command_pool::vk_command_pool graphics_device queue_family_index flags\n");

    vk::CommandPoolCreateInfo create_info (flags, queue_family_index);
    command_pool = graphics_device.createCommandPool (create_info);
    this->graphics_device = graphics_device;
}

vk_command_pool::vk_command_pool (vk_command_pool&& other) noexcept
{
    OutputDebugString (L"vk_command_pool::vk_command_pool Move constructor\n");

    *this = std::move (other);
}

vk_command_pool& vk_command_pool::operator=(vk_command_pool&& other) noexcept
{
    OutputDebugString (L"vk_command_pool::vk_command_pool Move assignment\n");

    command_pool = other.command_pool;
    graphics_device = other.graphics_device;

    other.command_pool = nullptr;
    other.graphics_device = nullptr;

    return *this;
}

vk_command_pool::~vk_command_pool ()
{
    OutputDebugString (L"vk_command_pool::~vk_command_pool\n");

    if (graphics_device != nullptr && command_pool != nullptr)
    {
        graphics_device.destroyCommandPool (command_pool);
    }
}

vk_image::vk_image ()
{
    OutputDebugString (L"vk_image::vk_image\n");
}

vk_image::vk_image (const vk::Device& graphics_device, const uint32_t& queue_family_index, const vk::Extent3D& extent, const uint32_t& array_layers, const vk::Format& format, const vk::ImageLayout& image_layout, vk::SharingMode sharing_mode, vk::ImageUsageFlags usage)
{
    OutputDebugString (L"vk_image::vk_image graphics_device queue_family_index extent array_layers format image_layout sharing_mode usage\n");

    vk::ImageCreateInfo create_info ({}, vk::ImageType::e2D, format, extent, 1, array_layers, vk::SampleCountFlagBits::e1 , {}, usage, sharing_mode, 1, &queue_family_index, image_layout);

    image = graphics_device.createImage (create_info);

    this->graphics_device = graphics_device;
}

vk_image::vk_image (vk_image&& other) noexcept
{
    OutputDebugString (L"vk_image::vk_image Move constructor\n");

    *this = std::move (other);
}

vk_image& vk_image::operator= (vk_image&& other) noexcept
{
    OutputDebugString (L"vk_image::vk_image Move assignment\n");
    image = other.image;
    graphics_device = other.graphics_device;

    other.image = nullptr;
    other.graphics_device = nullptr;

    return *this;
}

vk_image::~vk_image () 
{
    OutputDebugString (L"vk_image::~vk_image\n");

    if (graphics_device != nullptr && image != nullptr)
    {
        graphics_device.destroy (image);
    }
}

vk_image_view::vk_image_view ()
{
    OutputDebugString (L"vk_image_view::vk_image_view\n");
}

vk_image_view::vk_image_view (const vk::Device& graphics_device, const vk::Image& image, const vk::Format& format)
{
    OutputDebugString (L"vk_image_view::vk_image_view graphics_device image format\n");

    vk::ComponentMapping component_mapping (vk::ComponentSwizzle::eIdentity);
    vk::ImageSubresourceRange subresource_range (vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
    vk::ImageViewCreateInfo create_info ({}, image, vk::ImageViewType::e2D, format, component_mapping, subresource_range);

    image_view = graphics_device.createImageView (create_info);
    this->graphics_device = graphics_device;
}

vk_image_view::vk_image_view (vk_image_view&& other) noexcept
{
    OutputDebugString (L"vk_image_view::vk_image_view Move constructor\n");

    *this = std::move (other);
}

vk_image_view& vk_image_view::operator=(vk_image_view&& other) noexcept
{
    OutputDebugString (L"vk_image_view::vk_image_view Move assignment\n");

    image_view = other.image_view;
    graphics_device = other.graphics_device;

    other.image_view = nullptr;
    other.graphics_device = nullptr;    

    return *this;
}

vk_image_view::~vk_image_view ()
{
    OutputDebugString (L"vk_image_view::~vk_image_view\n");

    if (graphics_device != nullptr && image_view != nullptr)
    {
        graphics_device.destroyImageView (image_view);
    }
}

vk_physical_device::vk_physical_device (const vk::Instance& instance)
{
    OutputDebugString (L"vk_physical_device::vk_physical_device\n");

    physical_device = instance.enumeratePhysicalDevices ().at (0);

    memory_properties = physical_device.getMemoryProperties ();
    limits = physical_device.getProperties ().limits;
}

vk_queue_family_indices::vk_queue_family_indices (const vk::PhysicalDevice& physical_device)
{
    OutputDebugString (L"vk_queue_family_indices::vk_queue_family_indices physical_device\n");

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

vk_device_queues::vk_device_queues (const vk::PhysicalDevice& physical_device, const vk::Device& graphics_device, const vk_queue_family_indices* queue_family_indices, const std::vector<uint32_t>& queue_indices)
{
    OutputDebugString (L"vk_device_queues::vk_device_queues physical_device, graphics_device, queue_family_indices, queue_indices\n");

    std::vector<vk::QueueFamilyProperties> queue_family_properties = physical_device.getQueueFamilyProperties ();

    graphics_queue = graphics_device.getQueue (queue_family_indices->graphics_queue_family_index, queue_indices.at (0) > queue_family_properties.at (queue_family_indices->graphics_queue_family_index).queueCount ? queue_family_properties.at (queue_family_indices->graphics_queue_family_index).queueCount - 1 : queue_indices.at (0));
    compute_queue = graphics_device.getQueue (queue_family_indices->compute_queue_family_index, queue_indices.at (1) > queue_family_properties.at (queue_family_indices->compute_queue_family_index).queueCount ? queue_family_properties.at (queue_family_indices->compute_queue_family_index).queueCount - 1 : queue_indices.at (1));
    transfer_queue = graphics_device.getQueue (queue_family_indices->transfer_queue_family_index, queue_indices.at (2) > queue_family_properties.at (queue_family_indices->transfer_queue_family_index).queueCount ? queue_family_properties.at (queue_family_indices->transfer_queue_family_index).queueCount - 1 : queue_indices.at (2));
}

vk_queue_info::vk_queue_info (const vk_queue_family_indices* queue_family_indices)
{
    OutputDebugString (L"vk_queue_info::vk_queue_info queue_family_indices\n");

    std::vector<size_t> family_indices = { queue_family_indices->graphics_queue_family_index, queue_family_indices->compute_queue_family_index, queue_family_indices->transfer_queue_family_index };
    std::map<size_t, size_t> family_indices_queue_count;

    for (const auto& family_index : family_indices)
    {
        family_indices_queue_count[family_index] = std::count (family_indices.begin (), family_indices.end (), family_index);
    }

    for (const auto& family_index_family_count : family_indices_queue_count)
    {
        for (size_t i = 0; i < family_index_family_count.second; ++i)
        {
            queue_indices.push_back (i);
        }
    }

    float queue_priorities = 1.f;

    for (const auto& family_index_queue_count : family_indices_queue_count)
    {
        queue_create_infos.emplace_back (vk::DeviceQueueCreateInfo ({}, family_index_queue_count.first, family_index_queue_count.second, &queue_priorities));
    }
}

vk_buffer::vk_buffer ()
{
    OutputDebugString (L"vk_buffer::vk_buffer\n");
}

vk_buffer::vk_buffer (const vk::Device& graphics_device, const vk::DeviceSize& size, const vk::BufferUsageFlags& usage, const vk::SharingMode& sharing_mode, const uint32_t& graphics_queue_family_index)
{
    OutputDebugString (L"vk_buffer::vk_buffer graphics_device size usage sharing_mode graphics_queue_family_index\n");

    vk::BufferCreateInfo create_info ({}, size, usage, sharing_mode);

    buffer = graphics_device.createBuffer (create_info);
    this->graphics_device = graphics_device;
}

vk_buffer::vk_buffer (vk_buffer&& other) noexcept
{
    OutputDebugString (L"vk_buffer::vk_buffer Move constructor\n");

    *this = std::move (other);
}

vk_buffer& vk_buffer::operator=(vk_buffer&& other) noexcept
{
    OutputDebugString (L"vk_buffer::vk_buffer Move assignment\n");

    buffer = other.buffer;
    graphics_device = other.graphics_device;

    other.buffer = nullptr;
    other.graphics_device = nullptr;

    return *this;
}

vk_buffer::~vk_buffer ()
{
    OutputDebugString (L"vk_buffer::~vk_buffer\n");

    if (graphics_device != nullptr && buffer != nullptr)
    {
        graphics_device.destroyBuffer (buffer);
    }
}

void vk_buffer::bind_memory (const vk::DeviceMemory& memory, const vk::DeviceSize& offset)
{
    OutputDebugString (L"vk_buffer::bind_memory memory offset\n");

    graphics_device.bindBufferMemory (buffer, memory, offset);
}

void vk_buffer::copy_from (const vk::Buffer& src_buffer, const vk::DeviceSize& size, const vk::CommandPool& command_pool, const vk::Queue& transfer_queue)
{
    OutputDebugString (L"vk_buffer::copy_from src_buffer transfer_queue\n");

    vk::BufferCopy buffer_copy (0, 0, size);

    vk_command_buffer copy_cmd_buffer (graphics_device, command_pool);
    copy_cmd_buffer.begin ();
    copy_cmd_buffer.command_buffer.copyBuffer (src_buffer, buffer, buffer_copy);
    copy_cmd_buffer.end ();

    vk_queue one_time_submit_queue (transfer_queue, graphics_device);
    one_time_submit_queue.submit (copy_cmd_buffer.command_buffer);
}

vk_device_memory::vk_device_memory ()
{
    OutputDebugString (L"vk_device_memory::vk_device_memory\n");
}

vk_device_memory::vk_device_memory (const vk::Device& graphics_device, const vk::Buffer& buffer, const vk::PhysicalDeviceMemoryProperties& memory_properties, vk::MemoryPropertyFlags required_types)
{
    OutputDebugString (L"vk_device_memory::vk_device_memory graphics_device buffer memory_properties required_types\n");

    vk::MemoryRequirements memory_requirements = graphics_device.getBufferMemoryRequirements (buffer);

    uint32_t memory_type_index = 0;

    for (uint32_t i = 0; i < memory_properties.memoryTypeCount; ++i)
    {
        if (memory_requirements.memoryTypeBits & (1 << i) && required_types & memory_properties.memoryTypes[i].propertyFlags)
        {
            memory_type_index = i;
            break;
        }
    }

    vk::MemoryAllocateInfo allocate_info (memory_requirements.size, memory_type_index);

    device_memory = graphics_device.allocateMemory (allocate_info);
    this->graphics_device = graphics_device;
}

vk_device_memory::vk_device_memory (const vk::Device& graphics_device, const vk::ArrayProxy<vk::Image>& images, const vk::PhysicalDeviceMemoryProperties& memory_properties, vk::MemoryPropertyFlags required_types)
{
    OutputDebugString (L"vk_device_memory::vk_device_memory graphics_device images memory_properties required_types\n");

    vk::DeviceSize total_size = 0;
    uint32_t memory_type_bits = 0;

    vk::MemoryRequirements memory_requirements;
    for (const auto& image : images)
    {
        memory_requirements = graphics_device.getImageMemoryRequirements (image);
        total_size += memory_requirements.size;
    }

    uint32_t memory_type_index = 0;

    for (uint32_t i = 0; i < memory_properties.memoryTypeCount; ++i)
    {
        if (memory_requirements.memoryTypeBits * (1 << i) && required_types & memory_properties.memoryTypes[i].propertyFlags)
        {
            memory_type_index = i;
            break;
        }
    }

    vk::MemoryAllocateInfo allocate_info (total_size, memory_type_index);
    
    device_memory = graphics_device.allocateMemory (allocate_info);
    this->graphics_device = graphics_device;
}

vk_device_memory::vk_device_memory (vk_device_memory&& other) noexcept
{
    OutputDebugString (L"vk_device_memory::vk_device_memory Move constructor\n");

    *this = std::move (other);
}

vk_device_memory& vk_device_memory::operator=(vk_device_memory&& other) noexcept
{
    OutputDebugString (L"vk_device_memory::vk_device_memory Move assignment\n");

    device_memory = other.device_memory;
    graphics_device = other.graphics_device;

    other.device_memory = nullptr;
    other.graphics_device = nullptr;

    return *this;
}

vk_device_memory::~vk_device_memory () noexcept
{
    OutputDebugString (L"vk_device_memory::~vk_device_memory\n");

    if (graphics_device != nullptr && device_memory != nullptr)
    {
        graphics_device.freeMemory (device_memory);
    }
}

void vk_device_memory::bind_buffer (const vk::Buffer& buffer, const vk::DeviceSize& offset)
{
    OutputDebugString (L"vk_device_memory::bind_buffer buffer offset\n");

    graphics_device.bindBufferMemory (buffer, device_memory, offset);
}

void vk_device_memory::bind_images (const std::map<vk::Image, vk::DeviceSize>& images_offsets)
{
    OutputDebugString (L"vk_device_memory::bind_images images offsets\n");

    for (const auto& image_offset : images_offsets)
    {
        graphics_device.bindImageMemory (image_offset.first, device_memory, image_offset.second);
    }
}

void vk_device_memory::map_data (const std::vector<unsigned char>& data, const vk::DeviceSize& offset)
{
    OutputDebugString (L"vk_device_memory::map_data data\n");

    HANDLE mapped_data = graphics_device.mapMemory (device_memory, offset, data.size ());
    std::memcpy (mapped_data, data.data (), data.size ());
    graphics_device.unmapMemory (device_memory);
}

vk_command_buffer::vk_command_buffer ()
{
    OutputDebugString (L"vk_command_buffer::vk_command_buffer\n");
}

vk_command_buffer::vk_command_buffer (const vk::Device graphics_device, const vk::CommandPool& command_pool)
{
    OutputDebugString (L"vk_command_buffer::vk_comamnd_buffer graphics_device command_pool\n");

    vk::CommandBufferAllocateInfo allocate_info (command_pool, {}, 1);
    command_buffer = graphics_device.allocateCommandBuffers (allocate_info)[0];

    this->graphics_device = graphics_device;
    this->command_pool = command_pool;
}

vk_command_buffer::vk_command_buffer (vk_command_buffer&& other) noexcept
{
    OutputDebugString (L"vk_command_buffer::vk_command_buffer Move constructor\n");

    *this = std::move (other);
}

vk_command_buffer& vk_command_buffer::operator=(vk_command_buffer&& other) noexcept
{
    OutputDebugString (L"vk_command_buffer::vk_command_buffer Move assignment\n");

    command_buffer = other.command_buffer;
    command_pool = other.command_pool;
    graphics_device = other.graphics_device;

    other.command_buffer = nullptr;
    other.command_pool = nullptr;
    other.graphics_device = nullptr;

    return *this;
}

vk_command_buffer::~vk_command_buffer () noexcept
{
    OutputDebugString (L"vk_command_buffer::~vk_command_buffer\n");

    if (command_buffer != nullptr && command_pool != nullptr && graphics_device != nullptr)
    {
        graphics_device.freeCommandBuffers (command_pool, command_buffer);
    }
}

void vk_command_buffer::begin ()
{
    OutputDebugString (L"vk_command_buffer::begin\n");

    vk::CommandBufferBeginInfo begin_info (vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
    command_buffer.begin (begin_info);
}

void vk_command_buffer::end ()
{
    OutputDebugString (L"vk_command_buffer::end\n");

    command_buffer.end ();
}


vk_queue::vk_queue (const vk::Queue& queue, const vk::Device& graphics_device) : queue (queue), graphics_device (graphics_device)
{
    OutputDebugString (L"vk_queue::vk_queue queue graphics_device\n");
}

void vk_queue::submit (const vk::ArrayProxy<vk::CommandBuffer>& command_buffers)
{
    OutputDebugString (L"vk_queue::submit command_buffers\n");

    vk::SubmitInfo submit_info ({}, {}, {}, command_buffers.size (), command_buffers.data ());

    queue.submit (submit_info, nullptr);
    queue.waitIdle ();
}
