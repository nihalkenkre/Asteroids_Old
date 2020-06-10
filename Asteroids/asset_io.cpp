#include "asset_io.hpp"
#include "utils.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>
#include <string>


asset_io::asset_io (const std::vector<std::string>& file_paths)
{
    OutputDebugString (L"asset_io::asset_io\n");

    tinygltf::TinyGLTF loader;

    for (const auto& file_path : file_paths)
    {
        if (file_path.find_last_of (".glb") == std::string::npos)
        {
            continue;
        }

        tinygltf::Model model;
        if (!loader.LoadBinaryFromFile (&model, nullptr, nullptr, utils::get_full_file_path (file_path)))
        {
            continue;
        }

        models.push_back (model);
    }

    create_images ();
    create_materials ();
    create_static_meshes ();
}

asset_io::~asset_io ()
{
    OutputDebugString (L"asset_io::~asset_io\n");
}

std::vector<vk_image> asset_io::get_images ()
{
    return images;
}

std::vector<vk_material> asset_io::get_materials ()
{
    return materials;
}

std::vector<vk_static_mesh> asset_io::get_static_meshes ()
{
    return static_meshes;
}

void asset_io::create_images ()
{
    images.reserve (5);

    for (const auto& current_model : models)
    {
        for (const auto& current_image : current_model.images)
        {
            images.push_back (vk_image (current_image));
        }
    }
}

void asset_io::create_materials ()
{
    materials.reserve (5);
}

void asset_io::create_static_meshes ()
{
    static_meshes.reserve (5);
}
    