#include "scene_graphics.hpp"
#include "static_meshes.hpp"
#include "common_graphics.hpp"
#include "vk_utils.hpp"

#include <Windows.h>

scene_graphics::scene_graphics (const static_meshes* meshes, const common_graphics* c_graphics)
{
    OutputDebugString (L"scene_graphics::scene_graphics meshes\n");

    vk::DeviceSize total_data_size = 0;

    for (const auto& mesh : meshes->meshes)
    {
        for (auto& primitive : mesh.alpha_graphics_primitives->primitives)
        {
            primitive.positions_offset = total_data_size;
            total_data_size += primitive.positions.size ();

            primitive.uv0s_offset = total_data_size;
            total_data_size += primitive.uv0s.size ();

            primitive.normals_offset = total_data_size;
            total_data_size += primitive.normals.size ();

            primitive.indices_offset = total_data_size;
            total_data_size += primitive.indices.size ();

            primitive.mat->base_image->image_data_offset = total_data_size;
            total_data_size += primitive.mat->base_image->image_data.size ();
        }
    }

    vk_buffer staging_buffer (c_graphics->graphics_device->graphics_device, total_data_size, vk::BufferUsageFlagBits::eTransferSrc, vk::SharingMode::eExclusive, 0);
}
