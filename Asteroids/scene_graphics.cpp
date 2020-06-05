#include "scene_graphics.hpp"
#include "common_graphics.hpp"
#include "utils.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>


scene_graphics::scene_graphics ()
{
}

scene_graphics::~scene_graphics ()
{
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
    size_t total_size = 0;

    for (const auto& model : models)
    {
        for (const auto& image : model.images)
        {
            vk::ImageCreateInfo create_info ({}, vk::ImageType::e2D, vk::Format::eR8G8B8A8Unorm, { (uint32_t)image.width, (uint32_t)image.height, 1 }, 1, 0, vk::SampleCountFlagBits::e1, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst, vk::SharingMode::eExclusive);
            images.emplace (common_graphics::graphics_device->createImageUnique (create_info));
        }
    }
}
