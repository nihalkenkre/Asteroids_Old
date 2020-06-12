#include "vk_utils.hpp"

#include "common_graphics.hpp"


/*
vk::Buffer vk_utils::create_buffer (vk::DeviceSize size, vk::BufferUsageFlags usage)
{
    vk::BufferCreateInfo create_info ({}, size, usage, vk::SharingMode::eExclusive);

    return common_graphics::graphics_device->get_obj ().createBuffer (create_info);
}

vk::DeviceMemory vk_utils::create_memory_for_buffer (vk::Buffer buffer, vk::MemoryPropertyFlags required_memory_types)
{
    vk::MemoryRequirements memory_requirements = common_graphics::graphics_device->get_obj ().getBufferMemoryRequirements (buffer);
    vk::MemoryAllocateInfo allocate_info (memory_requirements.size, get_memory_type_index (memory_requirements, required_memory_types));
    vk::DeviceMemory out_device_memory = common_graphics::graphics_device->get_obj ().allocateMemory (allocate_info);
    common_graphics::graphics_device->get_obj ().bindBufferMemory (buffer, out_device_memory, 0);

    return std::move (out_device_memory);
}

void vk_utils::map_data_to_device_memory (vk::DeviceMemory& memory, vk::DeviceSize offset, vk::DeviceSize size, void* data_source)
{
    void* data = common_graphics::graphics_device->get_obj ().mapMemory (memory, offset, size);
    std::memcpy (data, data_source, (size_t)size);
    common_graphics::graphics_device->get_obj ().unmapMemory (memory);
}

void vk_utils::copy_buffer_to_buffer (vk::Buffer& src_buffer, vk::Buffer& dst_buffer, vk::DeviceSize size)
{
    vk::BufferCopy buffer_copy (0, 0, size);

    vk::CommandBuffer one_time_command_buffer = get_one_time_command_buffer (common_graphics::transfer_command_pool);
    one_time_command_buffer.copyBuffer (src_buffer, dst_buffer, buffer_copy);
    one_time_command_buffer.end ();
    
    submit_one_time_command_buffer (common_graphics::transfer_queue, one_time_command_buffer);
    common_graphics::graphics_device->get_obj ().freeCommandBuffers (common_graphics::transfer_command_pool, one_time_command_buffer);
}

vk::DeviceMemory vk_utils::create_memory_for_images (const std::vector<vk::Image>& images, vk::MemoryPropertyFlags required_memory_types)
{
    vk::MemoryAllocateInfo allocate_info = {};

    std::vector<vk::DeviceSize> offsets;
    offsets.reserve (images.size ());

    for (const auto& image : images)
    {
        offsets.push_back (allocate_info.allocationSize);
        vk::MemoryRequirements memory_requirements = common_graphics::graphics_device->get_obj ().getImageMemoryRequirements (image);
        allocate_info.allocationSize += memory_requirements.size;
        allocate_info.memoryTypeIndex = get_memory_type_index (memory_requirements, required_memory_types);
    }

    vk::DeviceMemory out_device_memory = common_graphics::graphics_device->get_obj ().allocateMemory (allocate_info);

    size_t current_index = 0;
    for (const auto& image : images)
    {
        common_graphics::graphics_device->get_obj ().bindImageMemory (image, out_device_memory, offsets.at (current_index));
        ++current_index;
    }

    return std::move (out_device_memory);
}

void vk_utils::change_image_layout (vk::Image& image, uint32_t num_layers, uint32_t src_family_queue_index, uint32_t dst_family_queue_index, vk::ImageLayout old_layout, vk::ImageLayout new_layout, vk::AccessFlags src_access, vk::AccessFlags dst_access, vk::PipelineStageFlags src_stage, vk::PipelineStageFlags dst_stage)
{
    vk::ImageSubresourceRange subresource_range (vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
    vk::ImageMemoryBarrier image_memory_barrier (src_access, dst_access, old_layout, new_layout, common_graphics::transfer_queue_family_index, common_graphics::transfer_queue_family_index, image, subresource_range);

    vk::CommandBuffer one_time_command_buffer = get_one_time_command_buffer (common_graphics::transfer_command_pool);
    one_time_command_buffer.pipelineBarrier (src_stage, dst_stage, vk::DependencyFlagBits::eDeviceGroup, 0, 0, image_memory_barrier);
    one_time_command_buffer.end ();

    submit_one_time_command_buffer (common_graphics::transfer_queue, one_time_command_buffer);
    common_graphics::graphics_device->get_obj ().freeCommandBuffers (common_graphics::transfer_command_pool, one_time_command_buffer);
}

void vk_utils::copy_buffer_to_image (vk::Buffer buffer, vk::Image& image, vk::DeviceSize offset, vk::Extent3D extent)
{
    vk::ImageSubresourceLayers subresource_layers (vk::ImageAspectFlagBits::eColor, 0, 0, 1);
    vk::BufferImageCopy buffer_image_copy (offset, {}, {}, subresource_layers, 0, extent);
    vk::CommandBuffer one_time_command_buffer = get_one_time_command_buffer (common_graphics::transfer_command_pool);
    one_time_command_buffer.copyBufferToImage (buffer, image, vk::ImageLayout::eTransferDstOptimal, buffer_image_copy);
    one_time_command_buffer.end ();
    submit_one_time_command_buffer (common_graphics::transfer_queue, one_time_command_buffer);
    common_graphics::graphics_device->get_obj ().freeCommandBuffers (common_graphics::transfer_command_pool, one_time_command_buffer);
}

vk::ImageView vk_utils::create_image_view (vk::Image& image)
{
    vk::ImageSubresourceRange subresource_range (vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
    vk::ImageViewCreateInfo create_info ({}, image, vk::ImageViewType::e2D, vk::Format::eR8G8B8A8Unorm, {}, subresource_range);
    return common_graphics::graphics_device->get_obj ().createImageView (create_info);
}

vk::CommandPool vk_utils::create_command_pool (size_t queue_family_index, vk::CommandPoolCreateFlags flags)
{
    vk::CommandPoolCreateInfo create_info (flags, queue_family_index);
    return common_graphics::graphics_device->get_obj ().createCommandPool (create_info);
}

void vk_utils::destroy_command_pool_and_buffers (vk::CommandPool command_pool)
{
}

void vk_utils::destroy_buffer_and_memory (vk::Buffer buffer, vk::DeviceMemory buffer_memory)
{
    common_graphics::graphics_device->get_obj ().destroyBuffer (buffer);
    common_graphics::graphics_device->get_obj ().freeMemory (buffer_memory);
}

uint32_t vk_utils::get_memory_type_index (vk::MemoryRequirements memory_requirements, vk::MemoryPropertyFlags required_memory_types)
{
    for (uint32_t i = 0; i < common_graphics::physical_device_memory_properties.memoryTypeCount; ++i)
    {
        if (memory_requirements.memoryTypeBits & (1 << i) && required_memory_types & common_graphics::physical_device_memory_properties.memoryTypes[i].propertyFlags)
        {
            return i;
        }
    }

    return -1;
}

vk::CommandBuffer vk_utils::get_one_time_command_buffer (vk::CommandPool command_pool)
{
    vk::CommandBufferAllocateInfo allocate_info (command_pool, vk::CommandBufferLevel::ePrimary, 1);
    vk::CommandBuffer out_command_buffer = std::move (common_graphics::graphics_device->get_obj ().allocateCommandBuffers (allocate_info).at (0));

    vk::CommandBufferBeginInfo begin_info (vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
    out_command_buffer.begin (begin_info);

    return out_command_buffer;
}

void vk_utils::submit_one_time_command_buffer (vk::Queue queue, vk::CommandBuffer command_buffer)
{
    vk::SubmitInfo submit_info ({}, {}, {}, 1, &command_buffer);
    queue.submit (submit_info, 0);

    queue.waitIdle ();
}
*/


vk_instance::vk_instance (const bool& is_validation_needed)
{
    OutputDebugString (L"vk_instance::vk_instance\n");

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

vk_instance::~vk_instance ()
{
    OutputDebugString (L"vk_instance::~vk_instance\n");

    instance.destroy ();
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


vk_debug_utils_messenger::vk_debug_utils_messenger (const vk::Instance& instance)
{
    OutputDebugString (L"vk_debug_utils_messenger::vk_debug_utils_messenger\n");

    pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance.getProcAddr ("vkCreateDebugUtilsMessengerEXT"));
    pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance.getProcAddr ("vkDestroyDebugUtilsMessengerEXT"));

    vk::DebugUtilsMessageSeverityFlagsEXT severity_flags (vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose);
    vk::DebugUtilsMessageTypeFlagsEXT message_type_flags (vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);

    debug_utils_messenger = instance.createDebugUtilsMessengerEXT (vk::DebugUtilsMessengerCreateInfoEXT ({}, severity_flags, message_type_flags, &debug_messenger_callback));
    this->instance = instance;
}

vk_debug_utils_messenger::~vk_debug_utils_messenger ()
{
    OutputDebugString (L"vk_debug_utils_messenger::~vk_debug_utils_messenger\n");

    instance.destroyDebugUtilsMessengerEXT (debug_utils_messenger);
}


vk_surface::vk_surface (const vk::Instance& instance, HINSTANCE h_instance, HWND h_wnd)
{
    OutputDebugString (L"vk_surface::vk_surface\n");

    vk::Win32SurfaceCreateInfoKHR create_info ({}, h_instance, h_wnd);
    surface = instance.createWin32SurfaceKHR (create_info);
    this->instance = instance;
}

vk_surface::~vk_surface ()
{
    OutputDebugString (L"vk_surface::~vk_surface\n");

    instance.destroySurfaceKHR (surface);
}


vk_graphics_device::vk_graphics_device (const vk::PhysicalDevice& physical_device, const std::vector<vk::DeviceQueueCreateInfo>& queue_create_infos)
{
    OutputDebugString (L"vk_graphics_device::vk_graphics_device\n");

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

vk_graphics_device::~vk_graphics_device ()
{
    OutputDebugString (L"vk_graphics_device::~vk_graphics_device\n");

    graphics_device.destroy ();
}


vk_swapchain::vk_swapchain (const vk::Device& graphics_device, const vk::SurfaceKHR& surface, const vk::SurfaceCapabilitiesKHR& surface_capabilities, const vk::SurfaceFormatKHR& surface_format, const vk::Extent2D& surface_extent, const vk::PresentModeKHR& present_mode)
{
    OutputDebugString (L"vk_swapchain::vk_swapchain\n");

    vk::SwapchainCreateInfoKHR swapchain_create_info ({}, surface, surface_capabilities.minImageCount + 1, surface_format.format, surface_format.colorSpace, surface_extent, 1, surface_capabilities.supportedUsageFlags, vk::SharingMode::eExclusive, 0, {}, surface_capabilities.currentTransform, vk::CompositeAlphaFlagBitsKHR::eOpaque, present_mode);
    swapchain = graphics_device.createSwapchainKHR (swapchain_create_info);
    
    this->graphics_device = graphics_device;
}

vk_swapchain::~vk_swapchain ()
{
    OutputDebugString (L"vk_swapchain::~vk_swapchain\n");

    graphics_device.destroySwapchainKHR (swapchain);
}

vk_command_pool::vk_command_pool (const vk::Device& graphics_device, const size_t& queue_family_index, const vk::CommandPoolCreateFlags& flags)
{
    OutputDebugString (L"vk_command_pool::vk_command_pool\n");

    vk::CommandPoolCreateInfo create_info (flags, queue_family_index);
    command_pool = graphics_device.createCommandPool (create_info);
    this->graphics_device = graphics_device;
}

vk_command_pool::~vk_command_pool ()
{
    OutputDebugString (L"vk_command_pool::~vk_command_pool\n");

    graphics_device.destroyCommandPool (command_pool);
}

vk_image_view::vk_image_view (const vk::Device& graphics_device, const vk::Image& image, const vk::Format& format)
{
    OutputDebugString (L"vk_image_view::vk_image_view\n");

    vk::ComponentMapping component_mapping (vk::ComponentSwizzle::eIdentity);
    vk::ImageSubresourceRange subresource_range (vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
    vk::ImageViewCreateInfo create_info ({}, image, vk::ImageViewType::e2D, format, component_mapping, subresource_range);

    image_view = graphics_device.createImageView (create_info);
    this->graphics_device = graphics_device;
}

vk_image_view::~vk_image_view ()
{
    OutputDebugString (L"vk_image_view::~vk_image_view\n");

    graphics_device.destroyImageView (image_view);
}
