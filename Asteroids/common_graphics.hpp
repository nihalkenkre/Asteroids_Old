#pragma once

#include <Windows.h>
#include <memory>
#include <vulkan/vulkan.hpp>

#include "vk_utils.hpp"


class common_graphics
{
public:
    common_graphics (HINSTANCE h_instance, HWND h_wnd);
    
    std::unique_ptr<vk_instance> instance;
    std::unique_ptr<vk_graphics_device> graphics_device;
    std::unique_ptr<vk_debug_utils_messenger> debug_utils_messenger;
    std::unique_ptr<vk_surface> surface;
    std::unique_ptr<vk_swapchain> swapchain;
    std::unique_ptr<vk_command_pool> transfer_command_pool;
    std::unique_ptr<vk_physical_device> physical_device;
    std::unique_ptr<vk_queue_family_indices> queue_family_indices;
    std::unique_ptr<vk_device_queues> device_queues;
    std::unique_ptr<vk_queue_info> queue_infos;

private:
    bool is_validation_needed = false;
};