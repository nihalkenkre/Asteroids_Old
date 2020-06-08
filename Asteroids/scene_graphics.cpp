#include "scene_graphics.hpp"
#include "common_graphics.hpp"
#include "utils.hpp"
#include "vk_utils.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>


scene_graphics::scene_graphics ()
{
    OutputDebugString (L"scene_graphics::scene_graphics\n");

    images.reserve (5);
    image_views.reserve (5);
}

scene_graphics::~scene_graphics ()
{
    OutputDebugString (L"scene_graphics::~scene_graphics\n");

    for (auto& image : images)
    {
        common_graphics::graphics_device.destroyImage (image);
    }
    for (auto& image_view : image_views)
    {
        common_graphics::graphics_device.destroyImageView (image_view);
    }
    common_graphics::graphics_device.freeMemory (images_memory);

    common_graphics::graphics_device.destroyBuffer (vertex_index_buffer);
    common_graphics::graphics_device.freeMemory (vertex_index_buffer_memory);
}

void scene_graphics::create_graphics_for_meshes (const std::vector<std::string>& file_paths)
{
    std::vector<tinygltf::Model> models;
    models.reserve (4);

    tinygltf::TinyGLTF loader;
    
    for (const auto& file_path : file_paths)
    {
        tinygltf::Model model;
        if (!loader.LoadBinaryFromFile (&model, nullptr, nullptr, utils::get_full_file_path (file_path)))
        {
            return;
        }

        models.push_back (model);
    }

    import_images (models);
    //import_materials (model.materials);
    //link_images_to_materials ();
    //import_graphics_primitives (model.nodes);
    //link_materials_to_graphics_primitives ();
}

void scene_graphics::create_graphics_for_images (const std::vector<std::string>& file_paths)
{
}

void scene_graphics::import_images (const std::vector<tinygltf::Model>& models)
{
    std::vector <tinygltf::Image> model_images;
    model_images.reserve (5);

    vk::DeviceSize total_size = 0;
    for (const auto& model : models)
    {
        for (const auto& image : model.images)
        {
            total_size += image.image.size ();
        }
    }

    vk::Buffer staging_buffer = vk_utils::create_buffer (total_size, vk::BufferUsageFlagBits::eTransferSrc);
    vk::DeviceMemory staging_buffer_memory = vk_utils::create_memory_for_buffer (staging_buffer, vk::MemoryPropertyFlagBits::eHostVisible);

    vk::DeviceSize current_offset = 0;
    for (const auto& model : models)
    {
        for (const auto& image : model.images)
        {
            vk_utils::map_data_to_device_memory (staging_buffer_memory, current_offset, image.image.size (), (void*)image.image.data ());

            vk::ImageCreateInfo image_create_info ({}, vk::ImageType::e2D, vk::Format::eR8G8B8A8Unorm, { static_cast<uint32_t>(image.width), static_cast<uint32_t>(image.height), 1 }, 1, 1, vk::SampleCountFlagBits::e1, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::SharingMode::eExclusive);
            vk::Image img = common_graphics::graphics_device.createImage (image_create_info);
            images.emplace_back (img);

            current_offset += image.image.size ();
            model_images.emplace_back (image);
        }
    }

    images_memory = vk_utils::create_memory_for_images (images, vk::MemoryPropertyFlagBits::eDeviceLocal);

    current_offset = 0;
    size_t current_index = 0;
    for (const auto& model_image : model_images)
    {
        vk_utils::change_image_layout (images.at (current_index), 1, common_graphics::transfer_queue_family_index, common_graphics::transfer_queue_family_index, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, vk::AccessFlagBits::eHostRead, vk::AccessFlagBits::eTransferWrite, vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer);
        vk_utils::copy_buffer_to_image (staging_buffer, images.at (current_index), current_offset, { static_cast<uint32_t>(model_image.width),  static_cast<uint32_t>(model_image.height), 1 });
        vk_utils::change_image_layout (images.at (current_index), 1, common_graphics::transfer_queue_family_index, common_graphics::graphics_queue_family_index, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal, vk::AccessFlagBits::eTransferWrite, vk::AccessFlagBits::eShaderRead, vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader);
        image_views.emplace_back (vk_utils::create_image_view (images.at (current_index)));

        current_offset += model_image.image.size ();
        ++current_index;
    }
}
