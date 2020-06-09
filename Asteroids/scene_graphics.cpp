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

    static_meshes.reserve (5);

    //images.reserve (5);
    //image_views.reserve (5);
}

scene_graphics::~scene_graphics ()
{
    OutputDebugString (L"scene_graphics::~scene_graphics\n");

    /*for (auto& image : images)
    {
        common_graphics::graphics_device.destroyImage (image);
    }

    for (auto& image_view : image_views)
    {
        common_graphics::graphics_device.destroyImageView (image_view);
    }*/

    common_graphics::graphics_device.freeMemory (images_memory);

    common_graphics::graphics_device.destroyBuffer (vertex_index_buffer);
    common_graphics::graphics_device.freeMemory (vertex_index_buffer_memory);
}

void scene_graphics::create_graphics_for_meshes (const std::vector<std::string>& file_paths)
{
    std::vector<tinygltf::Model> models;
    models.reserve (5);

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

    std::vector <unsigned char> all_mesh_data = get_static_mesh_data (models);
    std::vector <unsigned char> all_image_data = get_image_data (models);

    vk::Buffer staging_buffer = vk_utils::create_buffer (all_mesh_data.size (), vk::BufferUsageFlagBits::eTransferSrc);
    vk::DeviceMemory staging_buffer_memory = vk_utils::create_memory_for_buffer (staging_buffer, vk::MemoryPropertyFlagBits::eHostVisible);
    vk_utils::map_data_to_device_memory (staging_buffer_memory, 0, all_mesh_data.size (), (void*)all_mesh_data.data ());

    vertex_index_buffer = vk_utils::create_buffer (all_mesh_data.size (), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eIndexBuffer);
    vertex_index_buffer_memory = vk_utils::create_memory_for_buffer (vertex_index_buffer, vk::MemoryPropertyFlagBits::eDeviceLocal);
    vk_utils::copy_buffer_to_buffer (staging_buffer, vertex_index_buffer, all_mesh_data.size ());

    vk_utils::destroy_buffer_and_memory (staging_buffer, staging_buffer_memory);

    staging_buffer = vk_utils::create_buffer (all_image_data.size (), vk::BufferUsageFlagBits::eTransferSrc);
    staging_buffer_memory = vk_utils::create_memory_for_buffer (staging_buffer, vk::MemoryPropertyFlagBits::eHostVisible);
    vk_utils::map_data_to_device_memory (staging_buffer_memory, 0, all_image_data.size (), (void*) all_image_data.data ());

    vk_utils::destroy_buffer_and_memory (staging_buffer, staging_buffer_memory);
 
    import_static_meshes (models);
}

void scene_graphics::create_graphics_for_images (const std::vector<std::string>& file_paths)
{
}

std::vector <unsigned char> scene_graphics::get_static_mesh_data (const std::vector<tinygltf::Model>& models)
{
    std::vector <unsigned char> data;

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
                    data.insert (data.end (), model.buffers[buffer_view.buffer].data.begin() + accessor.byteOffset + buffer_view.byteOffset, model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset + buffer_view.byteLength);
                }
                
                auto normal_attribute = primitive.attributes.find ("NORMAL");
                if (normal_attribute != primitive.attributes.end ())
                {
                    auto accessor = model.accessors[normal_attribute->second];
                    auto buffer_view = model.bufferViews[accessor.bufferView];
                    data.reserve (data.capacity () + buffer_view.byteLength);
                    data.insert (data.end (), model.buffers[buffer_view.buffer].data.begin() + accessor.byteOffset + buffer_view.byteOffset, model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset + buffer_view.byteLength);
                }

                auto uv0_attribute = primitive.attributes.find ("TEXCOORD_0");
                if (uv0_attribute != primitive.attributes.end ())
                {
                    auto accessor = model.accessors[uv0_attribute->second];
                    auto buffer_view = model.bufferViews[accessor.bufferView];
                    data.reserve (data.capacity () + buffer_view.byteLength);
                    data.insert (data.end (), model.buffers[buffer_view.buffer].data.begin() + accessor.byteOffset + buffer_view.byteOffset, model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset + buffer_view.byteLength);
                }
            
                auto uv1_attribute = primitive.attributes.find ("TEXCOORD_1");
                if (uv1_attribute != primitive.attributes.end ())
                {
                    auto accessor = model.accessors[uv1_attribute->second];
                    auto buffer_view = model.bufferViews[accessor.bufferView];
                    data.reserve (data.capacity () + buffer_view.byteLength);
                    data.insert (data.end (), model.buffers[buffer_view.buffer].data.begin() + accessor.byteOffset + buffer_view.byteOffset, model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset + buffer_view.byteLength);
                }
                
                auto accessor = model.accessors[primitive.indices];
                auto buffer_view = model.bufferViews[accessor.bufferView];

                data.reserve (data.capacity() + buffer_view.byteLength);
                data.insert (data.end (), model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset, model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset + buffer_view.byteLength);
            }
        }
    }

    return data;
}

std::vector <unsigned char> scene_graphics::get_image_data (const std::vector<tinygltf::Model>& models)
{
    std::vector <unsigned char> data;

    for (const auto& model : models)
    {
        for (const auto& image : model.images)
        {
            data.reserve (data.capacity () + image.image.size ());
            data.insert (data.end (), image.image.begin(), image.image.end ());
        }
    }

    return data;
}

void scene_graphics::import_static_meshes (const std::vector<tinygltf::Model>& models)
{
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

            static_meshes.emplace_back (create_static_mesh (node.mesh, model));
        }
    }
}

vk_static_mesh scene_graphics::create_static_mesh (int mesh_index, const tinygltf::Model& model)
{
    vk_static_mesh out_static_mesh ({});
    auto current_static_mesh = model.meshes[mesh_index];
    out_static_mesh.name = current_static_mesh.name;

    out_static_mesh.opaque_graphics_primitives.reserve (1);
    out_static_mesh.alpha_graphics_primitives.reserve (1);
    out_static_mesh.blend_graphics_primitives.reserve (1);

    for (const auto& primitive : current_static_mesh.primitives)
    {
        auto current_material = model.materials[primitive.material];

        if (current_material.name.find ("OPAQUE") == 0)
        {
            out_static_mesh.opaque_graphics_primitives.emplace_back (create_static_primitive (primitive, model));
        }
        else if (current_material.name.find ("ALPHA") == 0) 
        {
            out_static_mesh.alpha_graphics_primitives.emplace_back (create_static_primitive (primitive, model));
        }
        else if (current_material.name.find ("BLEND") == 0)
        {
            out_static_mesh.blend_graphics_primitives.emplace_back (create_static_primitive (primitive, model));
        }
        else
        {
            out_static_mesh.opaque_graphics_primitives.emplace_back (create_static_primitive (primitive, model));
        }
    }

    return out_static_mesh;
}

vk_static_primitive scene_graphics::create_static_primitive (const tinygltf::Primitive& primitive, const tinygltf::Model& model)
{
    vk_static_primitive out_static_primitive ({});
    
    return out_static_primitive;
}

vk_material scene_graphics::create_material (int material_index, const tinygltf::Model& model)
{
    vk_material out_material ({});
    auto current_material = model.materials[material_index];
    out_material.name = current_material.name;
    return out_material;
}

vk_image scene_graphics::create_image (int image_index, const tinygltf::Model& model)
{
    vk_image out_image ({});
    auto current_image = model.images[image_index];
    out_image.name = current_image.name;
    return out_image;
}


/*void scene_graphics::import_images (const std::vector<tinygltf::Model>& models)
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

    vk_utils::destroy_buffer_and_memory (staging_buffer, staging_buffer_memory);
}

void scene_graphics::import_materials (const std::vector<tinygltf::Model>& models)
{
}
*/