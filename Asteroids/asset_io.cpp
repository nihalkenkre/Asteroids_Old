#include "asset_io.hpp"
#include "utils.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>


asset_io::asset_io (const std::vector<std::string>& file_paths)
{
    OutputDebugString (L"asset_io::asset_io\n");

    current_static_mesh_data_offset = 0;

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
    std::vector<unsigned char> data = get_static_mesh_raw_data ();
    std::vector<vk_static_mesh> meshes = get_vk_static_meshes ();

    return std::make_pair (data, meshes);
}

std::vector<unsigned char> asset_io::get_static_mesh_raw_data ()
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

    return data;
}

std::vector<vk_static_mesh> asset_io::get_vk_static_meshes ()
{
    std::vector<vk_static_mesh> meshes;
    meshes.reserve (5);

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
            
            vk_static_mesh vk_mesh;
            vk_mesh.name = current_mesh.name;

            vk_mesh.opaque_primitives.reserve (1);
            vk_mesh.alpha_primitives.reserve (1);
            vk_mesh.blend_primitives.reserve (1);

            for (const auto& primitive : current_mesh.primitives)
            {
                const auto material = model.materials[primitive.material];
                
                if (material.name.find ("OPAQUE") == 0)
                {
                    vk_mesh.opaque_primitives.emplace_back (create_primitive (primitive, model));
                }
                else if (material.name.find ("ALPHA") == 0)
                {
                    vk_mesh.alpha_primitives.emplace_back (create_primitive (primitive, model));
                }
                else if (material.name.find ("BLEND") == 0)
                {
                    vk_mesh.blend_primitives.emplace_back (create_primitive (primitive, model));
                }
            }

            meshes.emplace_back (vk_mesh);
        }
    }

    return meshes;
}

vk_static_primitive asset_io::create_primitive (const tinygltf::Primitive& primitive, const tinygltf::Model& model)
{
    vk_static_primitive vk_primitive;
    vk_primitive.material = create_material (model.materials [primitive.material]);

    auto position_attribute = primitive.attributes.find ("POSITION");
    if (position_attribute != primitive.attributes.end ())
    {
        vk_primitive.positions_offset = current_static_mesh_data_offset;
        auto accessor = model.accessors[position_attribute->second];
        auto buffer_view = model.bufferViews[accessor.bufferView];
        current_static_mesh_data_offset += buffer_view.byteLength;
    }

    auto normal_attribute = primitive.attributes.find ("NORMAL");
    if (normal_attribute != primitive.attributes.end ())
    {
        vk_primitive.normals_offset = current_static_mesh_data_offset;
        auto accessor = model.accessors[normal_attribute->second];
        auto buffer_view = model.bufferViews[accessor.bufferView];
        current_static_mesh_data_offset += buffer_view.byteLength;
    }

    auto uv0_attribute = primitive.attributes.find ("TEXCOORD_0");
    if (uv0_attribute != primitive.attributes.end ())
    {
        vk_primitive.uv0s_offset = current_static_mesh_data_offset;
        auto accessor = model.accessors[uv0_attribute->second];
        auto buffer_view = model.bufferViews[accessor.bufferView];
        current_static_mesh_data_offset += buffer_view.byteLength;
    }

    auto uv1_attribute = primitive.attributes.find ("TEXCOORD_1");
    if (uv1_attribute != primitive.attributes.end ())
    {
        vk_primitive.uv1s_offset = current_static_mesh_data_offset;
        auto accessor = model.accessors[uv1_attribute->second];
        auto buffer_view = model.bufferViews[accessor.bufferView];
        current_static_mesh_data_offset += buffer_view.byteLength;
    }

    vk_primitive.indices_offset = current_static_mesh_data_offset;
    auto accessor = model.accessors[primitive.indices];
    auto buffer_view = model.bufferViews[accessor.bufferView];
    current_static_mesh_data_offset += buffer_view.byteLength;

    return vk_primitive;
}

vk_material asset_io::create_material (const tinygltf::Material& material)
{
    vk_material out_material;
    out_material.name = material.name;

    return out_material;
}