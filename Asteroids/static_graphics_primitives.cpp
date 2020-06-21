#include "static_graphics_primitives.hpp"

#include <Windows.h>

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
}

static_graphics_primitives::static_graphics_primitives (const tinygltf::Node& graphics_node, const tinygltf::Model& model)
{
    OutputDebugString (L"static_graphics_primitives::static_graphics_primitives graphics_node model\n");

    primitives.reserve (5);

    for (const auto& primitive : model.meshes[graphics_node.mesh].primitives)
    {
        if (model.materials[primitive.material].name.find ("OPAQUE") != std::string::npos)
        {
            primitives.emplace_back (static_graphics_primitive (primitive, model));
        }
        else if (model.materials[primitive.material].name.find ("ALPHA") != std::string::npos)
        {
            primitives.emplace_back (static_graphics_primitive (primitive, model));
        }
        else if (model.materials[primitive.material].name.find ("BLEND") != std::string::npos)
        {
            primitives.emplace_back (static_graphics_primitive (primitive, model));
        }
    }
}
