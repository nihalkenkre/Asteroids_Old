#include "static_graphics_primitive.hpp"
#include "material.hpp"


static_graphics_primitive::static_graphics_primitive (const tinygltf::Primitive& primitive, const tinygltf::Model& model)
{
    OutputDebugString (L"static_graphics_primitive::static_graphics_primitive primitive model\n");

    auto position_attribute = primitive.attributes.find ("POSITION");
    if (position_attribute != primitive.attributes.end ())
    {
        auto accessor = model.accessors[position_attribute->second];
        auto buffer_view = model.bufferViews[accessor.bufferView];

        positions.resize (buffer_view.byteLength);
        std::copy (model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset, model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset + buffer_view.byteLength, positions.begin ());
    }

    auto normal_attribute = primitive.attributes.find ("NORMAL");
    if (normal_attribute != primitive.attributes.end ())
    {
        auto accessor = model.accessors[position_attribute->second];
        auto buffer_view = model.bufferViews[accessor.bufferView];

        normals.resize (buffer_view.byteLength);
        std::copy (model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset, model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset + buffer_view.byteLength, normals.begin ());
    }

    auto uv0s_attribute = primitive.attributes.find ("TEXCOORD_0");
    if (uv0s_attribute != primitive.attributes.end ())
    {
        auto accessor = model.accessors[position_attribute->second];
        auto buffer_view = model.bufferViews[accessor.bufferView];

        uv0s.resize (buffer_view.byteLength);
        std::copy (model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset, model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset + buffer_view.byteLength, uv0s.begin ());
    }

    auto accessor = model.accessors[primitive.indices];
    auto buffer_view = model.bufferViews[accessor.bufferView];

    indices.resize (buffer_view.byteLength);
    std::copy (model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset, model.buffers[buffer_view.buffer].data.begin () + accessor.byteOffset + buffer_view.byteOffset + buffer_view.byteLength, indices.begin ());

    mat = std::make_unique<material> (model.materials[primitive.material], model);
}