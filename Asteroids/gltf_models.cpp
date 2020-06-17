#include "gltf_models.hpp"
#include "utils.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>


gltf_models::gltf_models ()
{
    OutputDebugString (L"gltf_models::gltf_models\n");
}

gltf_models::gltf_models (const gltf_models& other)
{
    OutputDebugString (L"gltf_models::gltf_models Copy constructor\n");

    *this = other;
}

gltf_models& gltf_models::operator=(const gltf_models& other)
{
    OutputDebugString (L"gltf_models::gltf_models Copy assignment\n");
    
    models = other.models;

    return *this;
}

gltf_models::gltf_models (gltf_models&& other) noexcept
{
    OutputDebugString (L"gltf_models::gltf_models Move constructor\n");

    *this = std::move (other);
}

gltf_models& gltf_models::operator=(gltf_models&& other) noexcept
{
    OutputDebugString (L"gltf_models::gltf_models Move assignment\n");

    models = other.models;

    other.models.clear ();

    return *this;
}

gltf_models::gltf_models (const std::string& folder_path)
{
    OutputDebugString (L"gltf_models::gltf_models folder_path\n");

    models.reserve (5);

    std::string full_folder_path = utils::get_full_file_path (folder_path);
    std::vector<std::string> files = utils::get_all_files_in_folder (full_folder_path);

    for (const auto& file : files)
    {
        tinygltf::Model model;
        tinygltf::TinyGLTF loader;

        if (!loader.LoadBinaryFromFile (&model, nullptr, nullptr, file))
        {
            continue;
        }

        models.push_back (model);
    }
}

gltf_models::~gltf_models ()
{
    OutputDebugString (L"gltf_models::~gltf_models\n");
}