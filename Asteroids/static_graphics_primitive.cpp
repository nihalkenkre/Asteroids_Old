#include "static_graphics_primitive.hpp"
#include "material.hpp"


static_graphics_primitive::static_graphics_primitive (const tinygltf::Primitive& primitive, const tinygltf::Model& model)
{
    //OutputDebugString (L"static_graphics_primitive::static_graphics_primitive primitive model\n");

    auto position_attribute = primitive.attributes.find ("POSITION");
    if (position_attribute != primitive.attributes.end ())
    {
        auto accessor = model.accessors[position_attribute->second];
        auto buffer_view = model.bufferViews[accessor.bufferView];

        positions.resize (buffer_view.byteLength);
        std::copy (model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset, 
                   model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset + buffer_view.byteLength, 
                   positions.begin ());
    }

    auto normal_attribute = primitive.attributes.find ("NORMAL");
    if (normal_attribute != primitive.attributes.end ())
    {
        auto accessor = model.accessors[position_attribute->second];
        auto buffer_view = model.bufferViews[accessor.bufferView];

        normals.resize (buffer_view.byteLength);
        std::copy (model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset, 
                   model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset + buffer_view.byteLength, 
                   normals.begin ());
    }

    auto uv0s_attribute = primitive.attributes.find ("TEXCOORD_0");
    if (uv0s_attribute != primitive.attributes.end ())
    {
        auto accessor = model.accessors[position_attribute->second];
        auto buffer_view = model.bufferViews[accessor.bufferView];

        uv0s.resize (buffer_view.byteLength);
        std::copy (model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset, 
                   model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset + buffer_view.byteLength, 
                   uv0s.begin ());
    }

    auto accessor = model.accessors[primitive.indices];
    auto buffer_view = model.bufferViews[accessor.bufferView];

    if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
    {
        index_type = vk::IndexType::eUint8EXT;
    }
    else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
    {
        index_type = vk::IndexType::eUint16;
    }
    else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
    {
        index_type = vk::IndexType::eUint32;
    }

    index_count = accessor.count;    
    indices.resize (buffer_view.byteLength);
    std::copy (model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset, 
               model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset + buffer_view.byteLength, 
               indices.begin ());

    mat = std::make_unique<material> (model.materials[primitive.material], model);
}
