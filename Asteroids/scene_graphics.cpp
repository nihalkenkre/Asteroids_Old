#include "scene_graphics.hpp"
#include "static_meshes.hpp"
#include "common_graphics.hpp"
#include "vk_utils.hpp"

#include <Windows.h>

scene_graphics::scene_graphics (const static_meshes* meshes, const common_graphics* c_graphics)
{
    OutputDebugString (L"scene_graphics::scene_graphics meshes\n");

    vk::DeviceSize current_data_size = 0;

    std::vector<unsigned char> total_data;

    for (const auto& mesh : meshes->meshes)
    {
        for (auto& primitive : mesh.alpha_graphics_primitives->primitives)
        {
            total_data.resize (total_data.size () + primitive.positions.size () + primitive.normals.size () + primitive.uv0s.size () + primitive.indices.size () + primitive.mat->base_image->image_data.size ());

            primitive.positions_offset = current_data_size;
            std::copy (primitive.positions.begin (), primitive.positions.end (), total_data.begin () + static_cast<int>(current_data_size));
            current_data_size += primitive.positions.size ();

            primitive.normals_offset = current_data_size;
            std::copy (primitive.normals.begin (), primitive.normals.end (), total_data.begin () + static_cast<int>(current_data_size));
            current_data_size += primitive.normals.size ();

            primitive.uv0s_offset = current_data_size;
            std::copy (primitive.uv0s.begin (), primitive.uv0s.end (), total_data.begin () + static_cast<int>(current_data_size));
            current_data_size += primitive.uv0s.size ();

            primitive.indices_offset = current_data_size;
            std::copy (primitive.indices.begin (), primitive.indices.end (), total_data.begin () + static_cast<int>(current_data_size));
            current_data_size += primitive.indices.size ();

            primitive.mat->base_image->image_data_offset = current_data_size;
            std::copy (primitive.mat->base_image->image_data.begin (), primitive.mat->base_image->image_data.end (), total_data.begin () + static_cast<int>(current_data_size));
            current_data_size += primitive.mat->base_image->image_data.size ();
        }
    }

    vk_buffer staging_buffer (c_graphics->graphics_device->graphics_device, current_data_size, vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive, 0);
    vk_device_memory staging_buffer_memory (c_graphics->graphics_device->graphics_device, staging_buffer.buffer, c_graphics->physical_device->memory_properties, vk::MemoryPropertyFlagBits::eHostVisible);

    c_graphics->graphics_device->graphics_device.bindBufferMemory (staging_buffer.buffer, staging_buffer_memory.device_memory, 0);

    HANDLE data = c_graphics->graphics_device->graphics_device.mapMemory (staging_buffer_memory.device_memory, 0, current_data_size);
    std::memcpy (data, total_data.data (), total_data.size ());
    c_graphics->graphics_device->graphics_device.unmapMemory (staging_buffer_memory.device_memory);

    for (const auto& mesh : meshes->meshes)
    {
        for (auto& primitive : mesh.alpha_graphics_primitives->primitives)
        {
            primitive.mat->base_image->img = vk_image (c_graphics->graphics_device->graphics_device, c_graphics->queue_family_indices->graphics_queue_family_index, { primitive.mat->base_image->width, primitive.mat->base_image->height, 1 }, 1, vk::Format::eR8G8B8A8Unorm, vk::ImageLayout::eUndefined, vk::SharingMode::eExclusive, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled);
        }
    }
}
