#include "vk_utils.hpp"
#include "common_graphics.hpp"
#include "image.hpp"

#include <iostream>
#include <map>
#include <fstream>

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

    auto layer_iter = std::find_if (layer_properties.begin (), 
                                    layer_properties.end (), 
                                    [&](const vk::LayerProperties& layer_property) 
                                    { 
                                        return (strcmp (layer_property.layerName, "VK_LAYER_KHRONOS_validation") == 0); 
                                    });
    if (layer_iter != layer_properties.end ())
    {
        requested_instance_layers.push_back (layer_iter->layerName);
    }

    std::vector<vk::ExtensionProperties> extension_properties = vk::enumerateInstanceExtensionProperties ();
    auto extension_iter = std::find_if (extension_properties.begin (), 
                                        extension_properties.end (), 
                                        [&](const vk::ExtensionProperties& extension_property) 
                                        { 
                                            return (strcmp (extension_property.extensionName, VK_KHR_SURFACE_EXTENSION_NAME) == 0); 
                                        });
    if (extension_iter != extension_properties.end ())
    {
        requested_instance_extensions.push_back (extension_iter->extensionName);
    }

    extension_iter = std::find_if (extension_properties.begin (), 
                                   extension_properties.end (), 
                                   [&](const vk::ExtensionProperties& extension_property) 
                                   { 
                                       return (strcmp (extension_property.extensionName, "VK_KHR_win32_surface") == 0); 
                                   });
    if (extension_iter != extension_properties.end ())
    {
        requested_instance_extensions.push_back (extension_iter->extensionName);
    }

    if (is_validation_needed)
    {
        extension_iter = std::find_if (extension_properties.begin (), 
                                       extension_properties.end (), 
                                       [&](const vk::ExtensionProperties& extension_property) 
                                       { 
                                           return (strcmp (extension_property.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0); 
                                       });
        if (extension_iter != extension_properties.end ())
        {
            requested_instance_extensions.push_back (extension_iter->extensionName);
        }
    }

    vk::ApplicationInfo application_info ("Asteroids", VK_MAKE_VERSION (1, 0, 0), "AGE", VK_MAKE_VERSION (1, 0, 0), VK_API_VERSION_1_2);
    vk::InstanceCreateInfo instance_create_info ({}, 
                                                 &application_info, 
                                                 requested_instance_layers.size (), 
                                                 requested_instance_layers.data (), 
                                                 requested_instance_extensions.size (), 
                                                 requested_instance_extensions.data ());

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
        wchar_t wbuff[2048];
        swprintf_s (wbuff, 2048, L"Debug Messenger Callback: %hs\n", callback_data->pMessage);
        OutputDebugString (wbuff);
    
        std::cout << "Debug Messenger Callback:" << callback_data->pMessage << "\n";
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

    vk::DebugUtilsMessageSeverityFlagsEXT severity_flags (vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | 
                                                          vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | 
                                                          vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose);
    vk::DebugUtilsMessageTypeFlagsEXT message_type_flags (vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | 
                                                          vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | 
                                                          vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);

    debug_utils_messenger = instance.createDebugUtilsMessengerEXT (vk::DebugUtilsMessengerCreateInfoEXT ({}, 
                                                                                                         severity_flags, 
                                                                                                         message_type_flags, 
                                                                                                         &debug_messenger_callback));
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

vk_surface::vk_surface (const vk::Instance& instance, 
                        const HINSTANCE& h_instance, 
                        const HWND& h_wnd, 
                        const vk::PhysicalDevice& physical_device, 
                        const uint32_t& graphics_queue_family_index)
{
    OutputDebugString (L"vk_surface::vk_surface instance h_instance h_wnd\n");

    vk::Win32SurfaceCreateInfoKHR create_info ({}, h_instance, h_wnd);
    surface = instance.createWin32SurfaceKHR (create_info);

    bool is_supported = physical_device.getSurfaceSupportKHR (graphics_queue_family_index, surface);

    surface_capabilities = physical_device.getSurfaceCapabilitiesKHR (surface);
    surface_extent = surface_capabilities.currentExtent;

    std::vector<vk::SurfaceFormatKHR> surface_formats = physical_device.getSurfaceFormatsKHR (surface);

    auto format_iter = std::find_if (surface_formats.begin (), 
                                     surface_formats.end (), 
                                     [&](vk::SurfaceFormatKHR format) 
                                     { 
                                         return format == vk::Format::eB8G8R8A8Unorm; 
                                     });
    if (format_iter != surface_formats.end ())
    {
        surface_format = *format_iter;
    }

    std::vector<vk::PresentModeKHR> present_modes = physical_device.getSurfacePresentModesKHR (surface);

    auto present_mode_iter = std::find_if (present_modes.begin (), 
                                           present_modes.end (), 
                                           [&](const vk::PresentModeKHR& present_mode) 
                                           { 
                                               return present_mode == vk::PresentModeKHR::eMailbox; 
                                           });
    if (present_mode_iter != present_modes.end ())
    {
        present_mode = *present_mode_iter;
    }
    else
    {
        auto present_mode_iter = std::find_if (present_modes.begin (), 
                                               present_modes.end (), 
                                               [&](const vk::PresentModeKHR& present_mode) 
                                               { 
                                                   return present_mode == vk::PresentModeKHR::eFifo; 
                                               });
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

    auto iter = std::find_if (extension_properties.begin (), 
                              extension_properties.end (), 
                              [&](const vk::ExtensionProperties& extension_property) 
                              { 
                                  return strcmp (extension_property.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0; 
                              });
    if (iter != extension_properties.end ())
    {
        requested_device_extensions.push_back (VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }

    vk::DeviceCreateInfo device_create_info ({}, 
                                             queue_create_infos.size (), 
                                             queue_create_infos.data (), 
                                             0, 
                                             nullptr, 
                                             requested_device_extensions.size (), 
                                             requested_device_extensions.data ());
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

    vk::SwapchainCreateInfoKHR swapchain_create_info ({}, 
                                                      surface->surface, 
                                                      surface->surface_capabilities.minImageCount + 1, 
                                                      surface->surface_format.format, 
                                                      surface->surface_format.colorSpace, 
                                                      surface->surface_extent, 
                                                      1, 
                                                      surface->surface_capabilities.supportedUsageFlags, 
                                                      vk::SharingMode::eExclusive, 
                                                      0, 
                                                      {}, 
                                                      surface->surface_capabilities.currentTransform, 
                                                      vk::CompositeAlphaFlagBitsKHR::eOpaque, surface->present_mode);
    swapchain = graphics_device.createSwapchainKHR (swapchain_create_info);
    
    images = graphics_device.getSwapchainImagesKHR (swapchain);
    image_views.reserve (images.size ());
    for (const auto& image : images)
    {
        image_views.emplace_back (vk_image_view (graphics_device, image, surface->surface_format.format));
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

vk_command_pool::vk_command_pool (const vk::Device& graphics_device, 
                                  const size_t& queue_family_index, 
                                  const vk::CommandPoolCreateFlags& flags)
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

vk_image::vk_image (const vk::Device& graphics_device, 
                    const uint32_t& queue_family_index, 
                    const vk::Extent3D& extent, 
                    const uint32_t& array_layers, 
                    const vk::Format& format, 
                    const vk::ImageLayout& image_layout, 
                    vk::SharingMode sharing_mode, 
                    vk::ImageUsageFlags usage)
{
    OutputDebugString (L"vk_image::vk_image graphics_device queue_family_index extent array_layers format image_layout sharing_mode usage\n");

    vk::ImageCreateInfo create_info ({}, 
                                     vk::ImageType::e2D, 
                                     format, 
                                     extent, 
                                     1, 
                                     array_layers, 
                                     vk::SampleCountFlagBits::e1 , 
                                     {}, 
                                     usage, 
                                     sharing_mode, 
                                     1, 
                                     &queue_family_index, 
                                     image_layout);

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

void vk_image::change_layout (const vk::Queue& transfer_queue, 
                              const vk::CommandPool& transfer_command_pool, 
                              const uint32_t& src_queue_family_index, 
                              const uint32_t& dst_queue_family_index, 
                              const vk::Image& image, 
                              const uint32_t& num_layers, 
                              const vk::ImageLayout& old_layout, 
                              const vk::ImageLayout& new_layout, 
                              const vk::AccessFlags& src_access, 
                              const vk::AccessFlags& dst_access, 
                              const vk::PipelineStageFlags& src_pipeline_stage, 
                              const vk::PipelineStageFlags& dst_pipeline_stage)
{
    OutputDebugString (L"vk_image::change_layout\n");

    vk::ImageSubresourceRange subresource_range (vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
    vk::ImageMemoryBarrier image_memory_barrier (src_access, 
                                                 dst_access, 
                                                 old_layout, 
                                                 new_layout, 
                                                 src_queue_family_index, 
                                                 dst_queue_family_index, 
                                                 image, 
                                                 subresource_range);

    vk_command_buffers one_time_buffer (graphics_device, transfer_command_pool, 1);
    one_time_buffer.begin (vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
    one_time_buffer.command_buffers.at (0).pipelineBarrier (src_pipeline_stage, 
                                                            dst_pipeline_stage, 
                                                            vk::DependencyFlagBits::eDeviceGroup, 
                                                            {}, 
                                                            {}, 
                                                            image_memory_barrier); 
    one_time_buffer.end ();

    vk_queue one_time_queue (transfer_queue, graphics_device);
    one_time_queue.submit (one_time_buffer.command_buffers);
}

void vk_image::copy_from_buffer (const vk::Queue & transfer_queue, 
                                 const vk::CommandPool& transfer_command_pool, 
                                 const vk::DeviceSize& offset, 
                                 const vk::Buffer& buffer, 
                                 const vk::Extent3D& extent, 
                                 const uint32_t& num_layers)
{
    OutputDebugString (L"vk_image::copy_from_buffer\n");

    vk::ImageSubresourceLayers subresource_layers (vk::ImageAspectFlagBits::eColor, 0, 0, num_layers);
    vk::BufferImageCopy buffer_image_copy (offset, {}, {}, subresource_layers, {}, extent);

    vk_command_buffers one_time_buffer (graphics_device, transfer_command_pool, 1);
    one_time_buffer.begin (vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
    one_time_buffer.command_buffers.at (0).copyBufferToImage (buffer, 
                                                              image, 
                                                              vk::ImageLayout::eTransferDstOptimal, 
                                                              buffer_image_copy);
    one_time_buffer.end ();

    vk_queue one_time_queue (transfer_queue, graphics_device);
    one_time_queue.submit (one_time_buffer.command_buffers);
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
    vk::ImageViewCreateInfo create_info ({}, 
                                         image, 
                                         vk::ImageViewType::e2D, 
                                         format, 
                                         component_mapping, 
                                         subresource_range);

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

    graphics_queue_family_index = std::distance (queue_family_properties.begin (), 
                                                 std::find_if (queue_family_properties.begin (), 
                                                               queue_family_properties.end (), 
                                                               [&](const vk::QueueFamilyProperties& family_property) 
                                                               { 
                                                                   return (family_property.queueFlags & vk::QueueFlagBits::eGraphics); 
                                                               }
                                                               )
                                                 );

    auto compute_family_index_iter = std::find_if (queue_family_properties.begin (), 
                                                   queue_family_properties.end (), 
                                                   [&](const vk::QueueFamilyProperties& family_property) 
                                                   { 
                                                       return (family_property.queueFlags & vk::QueueFlagBits::eCompute) && (!(family_property.queueFlags & vk::QueueFlagBits::eGraphics));
                                                   });
    if (compute_family_index_iter != queue_family_properties.end ())
    {
        compute_queue_family_index = std::distance (queue_family_properties.begin (), compute_family_index_iter);
    }
    else
    {
        compute_queue_family_index = std::distance (queue_family_properties.begin (), 
                                                    std::find_if (queue_family_properties.begin (), 
                                                                  queue_family_properties.end (), 
                                                                  [&](const vk::QueueFamilyProperties& family_property) 
                                                                  { 
                                                                      return (family_property.queueFlags & vk::QueueFlagBits::eCompute); 
                                                                  }
                                                                  )
                                                    );
    }

    auto transfer_family_index_iter = std::find_if (queue_family_properties.begin (), 
                                                    queue_family_properties.end (), 
                                                    [&](const vk::QueueFamilyProperties& family_property) 
                                                    { 
                                                        return (family_property.queueFlags & vk::QueueFlagBits::eTransfer) && 
                                                               (!(family_property.queueFlags & vk::QueueFlagBits::eGraphics)) && 
                                                               (!(family_property.queueFlags & vk::QueueFlagBits::eCompute)); 
                                                    }
                                                    );
    if (transfer_family_index_iter != queue_family_properties.end ())
    {
        transfer_queue_family_index = std::distance (queue_family_properties.begin (), transfer_family_index_iter);
    }
    else
    {
        transfer_queue_family_index = std::distance (queue_family_properties.begin (), 
                                                     std::find_if (queue_family_properties.begin (), 
                                                                   queue_family_properties.end (), 
                                                                   [&](const vk::QueueFamilyProperties& family_property) 
                                                                   { 
                                                                       return (family_property.queueFlags & vk::QueueFlagBits::eTransfer); 
                                                                   }
                                                                   )
                                                     );
    }
}

vk_device_queues::vk_device_queues (const vk::PhysicalDevice& physical_device, 
                                    const vk::Device& graphics_device, 
                                    const vk_queue_family_indices* queue_family_indices, 
                                    const std::vector<uint32_t>& queue_indices)
{
    OutputDebugString (L"vk_device_queues::vk_device_queues physical_device, graphics_device, queue_family_indices, queue_indices\n");

    std::vector<vk::QueueFamilyProperties> queue_family_properties = physical_device.getQueueFamilyProperties ();

    graphics_queue = std::make_unique<vk_queue> (
            graphics_device.getQueue (
                queue_family_indices->graphics_queue_family_index, 
                queue_indices.at (0) > queue_family_properties.at (queue_family_indices->graphics_queue_family_index).queueCount ? 
                queue_family_properties.at (queue_family_indices->graphics_queue_family_index).queueCount - 1 : queue_indices.at (0)), 
                graphics_device);
    
    compute_queue = std::make_unique<vk_queue> (
            graphics_device.getQueue (
                queue_family_indices->compute_queue_family_index, 
                queue_indices.at (1) > queue_family_properties.at (queue_family_indices->compute_queue_family_index).queueCount ? 
                queue_family_properties.at (queue_family_indices->compute_queue_family_index).queueCount - 1 : queue_indices.at (1)),
                graphics_device);

    transfer_queue = std::make_unique<vk_queue> (
            graphics_device.getQueue (
                queue_family_indices->transfer_queue_family_index,
                queue_indices.at (2) > queue_family_properties.at (queue_family_indices->transfer_queue_family_index).queueCount ? 
                queue_family_properties.at (queue_family_indices->transfer_queue_family_index).queueCount - 1 : queue_indices.at (2)),
                graphics_device);
}

vk_queue_info::vk_queue_info (const vk_queue_family_indices* queue_family_indices)
{
    OutputDebugString (L"vk_queue_info::vk_queue_info queue_family_indices\n");

    std::vector<size_t> family_indices = { queue_family_indices->graphics_queue_family_index, 
                                           queue_family_indices->compute_queue_family_index, 
                                           queue_family_indices->transfer_queue_family_index };
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

vk_buffer::vk_buffer (const vk::Device& graphics_device, 
                      const vk::DeviceSize& size, 
                      const vk::BufferUsageFlags& usage, 
                      const vk::SharingMode& sharing_mode, 
                      const uint32_t& graphics_queue_family_index)
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

void vk_buffer::copy_from_buffer (const vk::Buffer& src_buffer, 
                                  const vk::DeviceSize& size, 
                                  const vk::CommandPool& command_pool, 
                                  const vk::Queue& transfer_queue)
{
    OutputDebugString (L"vk_buffer::copy_from_buffer src_buffer transfer_queue\n");

    vk::BufferCopy buffer_copy (0, 0, size);

    vk_command_buffers copy_cmd_buffer (graphics_device, command_pool, 1);
    copy_cmd_buffer.begin (vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
    copy_cmd_buffer.command_buffers.at (0).copyBuffer (src_buffer, buffer, buffer_copy);
    copy_cmd_buffer.end ();

    vk_queue one_time_submit_queue (transfer_queue, graphics_device);
    one_time_submit_queue.submit (copy_cmd_buffer.command_buffers);
}

vk_device_memory::vk_device_memory ()
{
    OutputDebugString (L"vk_device_memory::vk_device_memory\n");
}

vk_device_memory::vk_device_memory (const vk::Device& graphics_device, 
                                    const vk::Buffer& buffer, 
                                    const vk::PhysicalDeviceMemoryProperties& memory_properties, 
                                    vk::MemoryPropertyFlags required_types)
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

vk_device_memory::vk_device_memory (const vk::Device& graphics_device, 
                                    const vk::ArrayProxy<vk::Image>& images, 
                                    const vk::PhysicalDeviceMemoryProperties& memory_properties, 
                                    vk::MemoryPropertyFlags required_types)
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

vk_device_memory::vk_device_memory (const vk::Device& graphics_device, 
                                    const std::vector<image*>& images, 
                                    const vk::PhysicalDeviceMemoryProperties& memory_properties, 
                                    vk::MemoryPropertyFlags required_types)
{
    OutputDebugString (L"vk_device_memory::vk_device_memory graphics_device vk_images memory_properties required_types\n");

    vk::DeviceSize total_size = 0;
    uint32_t memory_type_bits = 0;

    vk::MemoryRequirements memory_requirements;
    for (const auto& image : images)
    {
        memory_requirements = graphics_device.getImageMemoryRequirements (image->img->image);
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

vk_command_buffers::vk_command_buffers ()
{
    OutputDebugString (L"vk_command_buffers::vk_command_buffers\n");
}

vk_command_buffers::vk_command_buffers (const vk::Device graphics_device, 
                                        const vk::CommandPool& command_pool, 
                                        const uint32_t& num_command_buffers)
{
    OutputDebugString (L"vk_command_buffers::vk_comamnd_buffer graphics_device command_pool num_command_buffers\n");

    vk::CommandBufferAllocateInfo allocate_info (command_pool, {}, num_command_buffers);
    command_buffers = graphics_device.allocateCommandBuffers (allocate_info);

    this->graphics_device = graphics_device;
    this->command_pool = command_pool;
}

vk_command_buffers::vk_command_buffers (vk_command_buffers&& other) noexcept
{
    OutputDebugString (L"vk_command_buffers::vk_command_buffers Move constructor\n");

    *this = std::move (other);
}

vk_command_buffers& vk_command_buffers::operator= (vk_command_buffers&& other) noexcept
{
    OutputDebugString (L"vk_command_buffers::vk_command_buffers Move assignment\n");

    command_buffers = std::move (other.command_buffers);
    command_pool = other.command_pool;
    graphics_device = other.graphics_device;

    other.command_pool = nullptr;
    other.graphics_device = nullptr;

    return *this;
}

vk_command_buffers::~vk_command_buffers () noexcept
{
    OutputDebugString (L"vk_command_buffers::~vk_command_buffers\n");

    if (command_pool != nullptr && graphics_device != nullptr)
    {
        graphics_device.freeCommandBuffers (command_pool, command_buffers);
    }
}

void vk_command_buffers::begin (const vk::CommandBufferUsageFlags& flags)
{
    OutputDebugString (L"vk_command_buffers::begin flags\n");

    vk::CommandBufferBeginInfo begin_info (flags);

    for (const auto& command_buffer : command_buffers)
    {
        command_buffer.begin (begin_info);
    }
}

void vk_command_buffers::draw (const vk::RenderPass& render_pass, 
                               const std::vector<vk_framebuffer>& framebuffers, 
                               const vk::Rect2D& render_area)
{
    OutputDebugString (L"vk_command_buffers::draw render_pass, framebuffers\n");

    std::vector<vk::ClearValue> clear_values;
    vk::ClearValue clear_value;
    clear_value.color = vk::ClearColorValue (std::array<float, 4> {1, 0, 0, 1});
    clear_values.push_back (clear_value);

    vk::RenderPassBeginInfo render_pass_begin_info (render_pass, {}, render_area, clear_values.size (), clear_values.data ());

    size_t index = 0;
    for (const auto& command_buffer : command_buffers)
    {
        render_pass_begin_info.framebuffer = framebuffers.at (index).framebuffer;
        command_buffer.beginRenderPass (render_pass_begin_info, vk::SubpassContents::eInline);
        command_buffer.endRenderPass ();

        ++index;
    }
}

void vk_command_buffers::end ()
{
    OutputDebugString (L"vk_command_buffers::end\n");

    for (const auto& command_buffer : command_buffers)
    {
        command_buffer.end ();
    }
}


vk_queue::vk_queue (const vk::Queue& queue, const vk::Device& graphics_device) : queue (queue), graphics_device (graphics_device)
{
    OutputDebugString (L"vk_queue::vk_queue queue graphics_device\n");
}

void vk_queue::submit (const std::vector<vk::CommandBuffer>& command_buffers) const
{
    OutputDebugString (L"vk_queue::submit command_buffers\n");

    vk::SubmitInfo submit_info ({}, {}, {}, command_buffers.size (), command_buffers.data ());

    queue.submit (submit_info, nullptr);
    queue.waitIdle ();
}

void vk_queue::submit (const vk::PipelineStageFlags& wait_stage_flags, 
                       const vk::CommandBuffer& command_buffer, 
                       const vk::Semaphore& wait_semaphore, 
                       const vk::Semaphore& signal_semphore) const
{
    OutputDebugString (L"vk_queue::submit wait_stage_flags command_buffers wait_semaphores signal_semaphores\n");

    vk::SubmitInfo submit_info (1, &wait_semaphore, &wait_stage_flags, 1, &command_buffer, 1, &signal_semphore);

    queue.submit (submit_info, nullptr);
}

void vk_queue::present (const vk::SwapchainKHR& swapchain, 
                        const uint32_t& image_index, 
                        const vk::Semaphore& wait_semaphore) const
{
    OutputDebugString (L"vk_queue::present swapchains image_indices wait_semaphores\n");

    vk::PresentInfoKHR present_info (1, &wait_semaphore, 1, &swapchain, &image_index);
    queue.presentKHR (present_info);
}

vk_descriptor_pool::vk_descriptor_pool ()
{
    OutputDebugString (L"vk_descriptor_pool::vk_descriptor_pool\n");
}

vk_descriptor_pool::vk_descriptor_pool (const std::vector<std::pair<vk::DescriptorType, size_t>>& descriptor_types_counts, 
                                        const uint32_t& max_sets)
{
    OutputDebugString (L"vk_descriptor_pool::vk_descriptor_pool types_counts max_sets\n");

    std::vector<vk::DescriptorPoolSize> pool_sizes;
    pool_sizes.reserve (descriptor_types_counts.size ());

    for (const auto& type_count : descriptor_types_counts)
    {
        pool_sizes.emplace_back (vk::DescriptorPoolSize (type_count.first, type_count.second));
    }

    vk::DescriptorPoolCreateInfo create_info ({}, max_sets, pool_sizes.size (), pool_sizes.data ());
    descriptor_pool = graphics_device.createDescriptorPool (create_info);
}

vk_descriptor_pool::vk_descriptor_pool (vk_descriptor_pool&& other) noexcept
{
    OutputDebugString (L"vk_descriptor_pool::vk_descriptor_pool Move constructor\n");

    *this = std::move (other);
}

vk_descriptor_pool& vk_descriptor_pool::operator=(vk_descriptor_pool&& other) noexcept
{
    OutputDebugString (L"vk_descriptor_pool::vk_descriptor_pool Move assignment\n");

    descriptor_pool = other.descriptor_pool;
    graphics_device = other.graphics_device;

    other.descriptor_pool = nullptr;
    other.graphics_device = nullptr;

    return *this;
}

vk_descriptor_pool::~vk_descriptor_pool () noexcept
{
    OutputDebugString (L"vk_descriptor_pool::~vk_descriptor_pool\n");

    if (descriptor_pool != nullptr && graphics_device != nullptr)
    {
        graphics_device.destroyDescriptorPool (descriptor_pool);
    }
}

vk_descriptor_set_layout::vk_descriptor_set_layout ()
{
    OutputDebugString (L"vk_descriptor_set_layout::vk_descriptor_set_layout\n");
}

vk_descriptor_set_layout::vk_descriptor_set_layout (const std::vector<vk::DescriptorSetLayoutBinding>& bindings)
{
    OutputDebugString (L"vk_descriptor_set_layout::vk_descriptor_set_layout bindings\n");

    vk::DescriptorSetLayoutCreateInfo create_info ({}, bindings.size (), bindings.data ());

    descriptor_set_layout = graphics_device.createDescriptorSetLayout (create_info);
}

vk_descriptor_set_layout::vk_descriptor_set_layout (vk_descriptor_set_layout&& other) noexcept
{
    OutputDebugString (L"vk_descriptor_set_layout::vk_descriptor_set_layout Move constructor\n");

    *this = std::move (other);
}

vk_descriptor_set_layout& vk_descriptor_set_layout::operator=(vk_descriptor_set_layout&& other) noexcept
{
    OutputDebugString (L"vk_descriptor_set_layout::vk_descriptor_set_layout Move assignment\n");

    descriptor_set_layout = other.descriptor_set_layout;
    graphics_device = other.graphics_device;

    other.descriptor_set_layout = nullptr;
    other.graphics_device = nullptr;

    return *this;
}

vk_descriptor_set_layout::~vk_descriptor_set_layout () noexcept
{
    OutputDebugString (L"vk_descriptor_set_layout::~vk_descriptor_set_layout\n");

    if (descriptor_set_layout != nullptr && graphics_device != nullptr)
    {
        graphics_device.destroyDescriptorSetLayout (descriptor_set_layout);
    }
}

vk_descriptor_sets::vk_descriptor_sets ()
{
    OutputDebugString (L"vk_descriptor_set::vk_descriptor_set\n");
}

vk_descriptor_sets::vk_descriptor_sets (const vk::DescriptorPool& descriptor_pool, 
                                        const std::vector<vk::DescriptorSetLayout>& set_layouts, 
                                        const uint32_t& num_sets)
{
    OutputDebugString (L"vk_descriptor_set::vk_descriptor_set descriptor_pool set_layouts\n");

    vk::DescriptorSetAllocateInfo allocate_info (descriptor_pool, set_layouts.size (), set_layouts.data ());

    descriptor_sets = graphics_device.allocateDescriptorSets (allocate_info);
}

vk_descriptor_sets::vk_descriptor_sets (vk_descriptor_sets&& other) noexcept
{
    OutputDebugString (L"vk_descriptor_set::vk_descriptor_set Move constructor\n");

    *this = std::move (other);
}

vk_descriptor_sets& vk_descriptor_sets::operator=(vk_descriptor_sets&& other) noexcept
{
    OutputDebugString (L"vk_descriptor_set::vk_descriptor_set Move assignment\n");

    descriptor_sets = std::move (other.descriptor_sets);
    descriptor_pool = other.descriptor_pool;
    graphics_device = other.graphics_device;

    other.descriptor_pool = nullptr;
    other.graphics_device = nullptr;

    return *this;
}

vk_descriptor_sets::~vk_descriptor_sets () noexcept
{
    OutputDebugString (L"vk_descriptor_set::~vk_descriptor_set\n");

    if (descriptor_pool != nullptr && graphics_device != nullptr)
    {
        graphics_device.freeDescriptorSets (descriptor_pool, descriptor_sets);
    }
}

vk_render_pass::vk_render_pass ()
{
    OutputDebugString (L"vk_render_pass::vk_render_pass\n");
}

vk_render_pass::vk_render_pass (const vk::Device& graphics_device, const vk::Format& chosen_format)
{
    OutputDebugString (L"vk_render_pass::vk_render_pass graphics_device chosen_format\n");

    vk::AttachmentDescription attachment_description ({}, 
                                                      chosen_format, 
                                                      vk::SampleCountFlagBits::e1, 
                                                      vk::AttachmentLoadOp::eClear, 
                                                      {}, 
                                                      vk::AttachmentLoadOp::eDontCare, 
                                                      vk::AttachmentStoreOp::eDontCare, 
                                                      vk::ImageLayout::eUndefined, 
                                                      vk::ImageLayout::ePresentSrcKHR);
    vk::AttachmentReference attachment_reference (0, vk::ImageLayout::eColorAttachmentOptimal);

    vk::SubpassDescription subpass_description ({}, vk::PipelineBindPoint::eGraphics, 0, nullptr, 1, &attachment_reference);

    vk::RenderPassCreateInfo create_info ({}, 1, &attachment_description, 1, &subpass_description);
    render_pass = graphics_device.createRenderPass (create_info);
    this->graphics_device = graphics_device;
}

vk_render_pass::vk_render_pass (vk_render_pass&& other) noexcept
{
    OutputDebugString (L"vk_render_pass::vk_render_pass Move constructor\n");

    *this = std::move (other);
}

vk_render_pass& vk_render_pass::operator=(vk_render_pass&& other) noexcept
{
    OutputDebugString (L"vk_render_pass::vk_render_pass Move assignment\n");

    render_pass = other.render_pass;
    graphics_device = other.graphics_device;

    other.render_pass = nullptr;
    other.graphics_device = nullptr;

    return *this;
}

vk_render_pass::~vk_render_pass () noexcept
{
    OutputDebugString (L"vk_render_pass::~vk_render_pass\n");

    if (render_pass != nullptr && graphics_device != nullptr)
    {
        graphics_device.destroyRenderPass (render_pass);
    }
}

vk_framebuffer::vk_framebuffer ()
{
    OutputDebugString (L"vk_framebuffer::vk_framebuffer\n");
}

vk_framebuffer::vk_framebuffer (const vk::Device& graphics_device, 
                                const vk::ImageView& image_view, 
                                const vk::RenderPass& render_pass, 
                                const vk::Extent2D& extent)
{
    OutputDebugString (L"vk_framebuffer::vk_framebuffer graphics_device imgae_view render_pass extext");

    vk::FramebufferCreateInfo create_info ({}, render_pass, 1, &image_view, extent.width, extent.height, 1);
    
    framebuffer = graphics_device.createFramebuffer (create_info);
    this->graphics_device = graphics_device;
}

vk_framebuffer::vk_framebuffer (vk_framebuffer&& other) noexcept
{
    OutputDebugString (L"vk_framebuffer::vk_framebuffer Move constructor\n");

    *this = std::move (other);
}

vk_framebuffer& vk_framebuffer::operator=(vk_framebuffer&& other) noexcept
{
    OutputDebugString (L"vk_framebuffer::vk_framebuffer Move assignment\n");

    framebuffer = other.framebuffer;
    graphics_device = other.graphics_device;

    other.framebuffer = nullptr;
    other.graphics_device = nullptr;

    return *this;
}

vk_framebuffer::~vk_framebuffer () noexcept
{
    OutputDebugString (L"vk_framebuffer::~vk_framebuffer\n");

    if (framebuffer != nullptr && graphics_device != nullptr)
    {
        graphics_device.destroyFramebuffer (framebuffer);
    }
}


vk_framebuffers::vk_framebuffers (const vk::Device& graphics_device, 
                                  const std::vector<vk_image_view>& image_views, 
                                  const vk::RenderPass& render_pass, 
                                  const vk::Extent2D& extent)
{
    OutputDebugString (L"vk_framebuffers::vk_framebuffers graphics_device image_views render_pass extent\n");

    framebuffers.reserve (image_views.size ());

    for (const auto& image_view : image_views)
    {
        framebuffers.emplace_back (vk_framebuffer (graphics_device, image_view.image_view, render_pass, extent));
    }
}


vk_graphics_pipeline::vk_graphics_pipeline ()
{
    OutputDebugString (L"vk_graphics_pipeline::vk_graphics_pipeline\n");
}

vk_graphics_pipeline::vk_graphics_pipeline (vk_graphics_pipeline&& other) noexcept
{
    OutputDebugString (L"vk_graphics_pipeline::vk_graphics_pipeline Move constructor\n");

    *this = std::move (other);
}

vk_graphics_pipeline& vk_graphics_pipeline::operator=(vk_graphics_pipeline&& other) noexcept
{
    OutputDebugString (L"vk_graphics_pipeline::vk_graphics_pipeline Move assignment\n");

    graphics_pipeline = other.graphics_pipeline;
    graphics_device = other.graphics_device;

    other.graphics_pipeline = nullptr;
    other.graphics_device = nullptr;

    return *this;
}

vk_graphics_pipeline::~vk_graphics_pipeline () noexcept
{
    OutputDebugString (L"vk_graphics_pipeline::~vk_graphics_pipeline\n");

    if (graphics_pipeline != nullptr && graphics_device != nullptr)
    {
        graphics_device.destroyPipeline (graphics_pipeline);
    }
}

vk_semaphore::vk_semaphore ()
{
    OutputDebugString (L"vk_semaphore::vk_semaphore\n");
}

vk_semaphore::vk_semaphore (const vk::Device& graphics_device)
{
    OutputDebugString (L"vk_semaphore::vk_semaphore graphics_device\n");

    semaphore = graphics_device.createSemaphore (vk::SemaphoreCreateInfo ());
    this->graphics_device = graphics_device;
}

vk_semaphore::vk_semaphore (vk_semaphore&& other) noexcept
{
    OutputDebugString (L"vk_semaphore::vk_semaphore Move constructor\n");

    *this = std::move (other);
}

vk_semaphore& vk_semaphore::operator= (vk_semaphore&& other) noexcept
{
    OutputDebugString (L"vk_semaphore::vk_semaphore Move assignment\n");

    semaphore = other.semaphore;
    graphics_device = other.graphics_device;

    other.semaphore = nullptr;
    other.graphics_device = nullptr;

    return *this;
}

vk_semaphore::~vk_semaphore () noexcept
{
    OutputDebugString (L"vk_semaphore::~vk_semaphore\n");

    if (semaphore != nullptr && graphics_device != nullptr)
    {
        graphics_device.destroySemaphore (semaphore);
    }
}


vk_semaphores::vk_semaphores (const vk::Device& graphics_device, const size_t& num_semaphores)
{
    OutputDebugString (L"vk_semaphores::vk_semaphores graphics_device\n");

    semaphores.reserve (num_semaphores);

    for (size_t i = 0; i < num_semaphores; ++i)
    {
        semaphores.emplace_back (vk_semaphore (graphics_device));
    }
}


vk_shader_module::vk_shader_module ()
{
    OutputDebugString (L"vk_shader_module::vk_shader_module\n");
}

vk_shader_module::vk_shader_module (const full_file_path& shader_path, const vk::Device& graphics_device, const vk::ShaderStageFlags& flags)
{
    OutputDebugString (L"vk_shader_module::vk_shader_module shader_path graphics_device flags\n");

    std::ifstream shader_file (shader_path.path, std::ios::in | std::ios::binary | std::ios::ate);
    
    if (!shader_file.good ())
    {
        throw shader_path.path + " file not good";
    }

    if (!shader_file.is_open ())
    {
        throw "could not open " + shader_path.path;
    }

    uint32_t size = (uint32_t) shader_file.tellg ();

    std::vector<char> buff (size);
    shader_file.seekg (0, std::ios::beg);
    shader_file.read (buff.data (), size);
    shader_file.close ();

    vk::ShaderModuleCreateInfo create_info ({}, size, reinterpret_cast<uint32_t*> (buff.data ()));

    shader_module = graphics_device.createShaderModule (create_info);
    this->graphics_device = graphics_device;
}

vk_shader_module::vk_shader_module (vk_shader_module&& other) noexcept
{
    OutputDebugString (L"vk_shader_module Move constructor\n");
    
    *this = std::move (other);
}

vk_shader_module& vk_shader_module::operator =(vk_shader_module&& other) noexcept
{
    OutputDebugString (L"vk_shader_module Move assignment\n");

    shader_module = other.shader_module;
    graphics_device = other.graphics_device;

    other.shader_module = nullptr;
    other.graphics_device = nullptr;

    return *this;
}

vk_shader_module::~vk_shader_module ()
{
    OutputDebugString (L"vk_shader_module::~vk_shader_module\n");

    if (shader_module != nullptr && graphics_device != nullptr)
    {
        graphics_device.destroyShaderModule (shader_module);
    }
}
