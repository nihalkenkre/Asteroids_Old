#include "static_graphics_primitives.hpp"

#include <Windows.h>


static_graphics_primitives::static_graphics_primitives (const tinygltf::Node& graphics_node, const MATERIAL_ALPHA_MODE& material_mode, const tinygltf::Model& model)
{
    //OutputDebugString (L"static_graphics_primitives::static_graphics_primitives graphics_node model\n");

    primitives.reserve (5);

    for (const auto& primitive : model.meshes[graphics_node.mesh].primitives)
    {
        switch (material_mode)
        {
        case MATERIAL_ALPHA_MODE::OPAQUE:
            if (model.materials[primitive.material].name.find ("OPAQUE") != std::string::npos)
            {
                primitives.emplace_back (static_graphics_primitive (primitive, model));
            }
            break;

        case MATERIAL_ALPHA_MODE::ALPHA:
            if (model.materials[primitive.material].name.find ("ALPHA") != std::string::npos)
            {
                primitives.emplace_back (static_graphics_primitive (primitive, model));
            }
            break;

        case MATERIAL_ALPHA_MODE::BLEND:
            if (model.materials[primitive.material].name.find ("BLEND") != std::string::npos)
            {
                primitives.emplace_back (static_graphics_primitive (primitive, model));
            }
            break;

        default:
            break;
        }
    }
}
