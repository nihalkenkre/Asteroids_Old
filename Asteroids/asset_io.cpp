#include "asset_io.hpp"
#include "utils.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>


asset_io::asset_io (const std::vector<std::string>& file_paths)
{
    OutputDebugString (L"asset_io::asset_io\n");

    tinygltf::TinyGLTF loader;

    for (const auto& file_path : file_paths)
    {
        tinygltf::Model model;
        if (!loader.LoadBinaryFromFile (&model, nullptr, nullptr, utils::get_full_file_path (file_path)))
        {
            continue;
        }

        models.push_back (model);
    }
}

asset_io::~asset_io ()
{
    OutputDebugString (L"asset_io::~asset_io\n");
}

std::pair<std::vector<unsigned char>, std::vector<vk_static_mesh>> asset_io::get_static_mesh_data ()
{
    std::vector<unsigned char> data;

    for (const auto& model : models)
    {
        for (const auto& node : model.nodes)
        {
            if (node.name.find ("CS_") == 0)
            {
                continue;
            }

            if (node.skin > 0)
            {
                continue;
            }

            auto current_mesh = model.meshes[node.mesh];

            for (const auto& primitive : current_mesh.primitives)
            {
                auto position_attribute = primitive.attributes.find ("POSITION");
                if (position_attribute != primitive.attributes.end ())
                {
                    auto accessor = model.accessors[position_attribute->second];
                    auto buffer_view = model.bufferViews[accessor.bufferView];
                    data.reserve (data.capacity () + buffer_view.byteLength);
                    data.insert (data.end (), model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset, model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset + buffer_view.byteLength);
                }

                auto normal_attribute = primitive.attributes.find ("NORMAL");
                if (normal_attribute != primitive.attributes.end ())
                {
                    auto accessor = model.accessors[normal_attribute->second];
                    auto buffer_view = model.bufferViews[accessor.bufferView];
                    data.reserve (data.capacity () + buffer_view.byteLength);
                    data.insert (data.end (), model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset, model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset + buffer_view.byteLength);
                }

                auto uv0_attribute = primitive.attributes.find ("TEXCOORD_0");
                if (uv0_attribute != primitive.attributes.end ())
                {
                    auto accessor = model.accessors[uv0_attribute->second];
                    auto buffer_view = model.bufferViews[accessor.bufferView];
                    data.reserve (data.capacity () + buffer_view.byteLength);
                    data.insert (data.end (), model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset, model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset + buffer_view.byteLength);
                }

                auto uv1_attribute = primitive.attributes.find ("TEXCOORD_1");
                if (uv1_attribute != primitive.attributes.end ())
                {
                    auto accessor = model.accessors[uv1_attribute->second];
                    auto buffer_view = model.bufferViews[accessor.bufferView];
                    data.reserve (data.capacity () + buffer_view.byteLength);
                    data.insert (data.end (), model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset, model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset + buffer_view.byteLength);
                }

                auto accessor = model.accessors[primitive.indices];
                auto buffer_view = model.bufferViews[accessor.bufferView];

                data.reserve (data.capacity () + buffer_view.byteLength);
                data.insert (data.end (), model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset, model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset + buffer_view.byteLength);
            }
        }
    }

    return std::make_pair (data, std::vector<vk_static_mesh> ());
}
