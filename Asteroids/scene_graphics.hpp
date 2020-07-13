#pragma once

#include "vk_utils.hpp"
#include "scene_assets.hpp"
#include <memory>


class common_graphics;

class scene_graphics
{
public:
    scene_graphics (const scene_assets* assets, const common_graphics* c_graphics);
    ~scene_graphics () noexcept;

    void main_loop ();

    void update_command_buffers (const vk::Extent2D& extent);
    void update_command_buffers (const vk::Extent2D& extent, const std::string& actor_name);

private:
    std::unique_ptr<vk_buffer> vb_ib;
    std::unique_ptr<vk_device_memory> vb_ib_memory;

    std::unique_ptr<vk_device_memory> image_memory;

    std::unique_ptr<vk_render_pass> render_pass;
    std::unique_ptr<vk_framebuffers> framebuffers;
    std::unique_ptr<vk_semaphores> signal_semaphores;
    std::unique_ptr<vk_semaphore> wait_semaphore;

    std::unique_ptr<vk_command_pool> graphics_command_pool;
    std::unique_ptr<vk_command_buffers> swapchain_command_buffers;

    std::unique_ptr<vk_shader_module> vertex_shader_module;
    std::unique_ptr<vk_shader_module> fragment_shader_module;

    const vk_graphics_device* graphics_device;
    const vk_swapchain* swapchain;
    const vk_queue* graphics_queue;

    const scene_assets* assets;
};
