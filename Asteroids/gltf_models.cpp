#include "gltf_models.hpp"
#include "utils.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>


gltf_models::gltf_models (const std::string& folder_path)
{
    OutputDebugString (L"gltf_models::gltf_models folder_path\n");

    models.reserve (5);

    full_file_path full_path (folder_path);
    files_in_folder files (full_path.path);

    for (const auto& file : files.files)
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
