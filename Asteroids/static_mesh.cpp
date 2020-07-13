#include "static_mesh.hpp"


static_mesh::static_mesh (const tinygltf::Node& graphics_node, const std::vector<tinygltf::Node>& physics_nodes, const tinygltf::Model& model)
{
    //OutputDebugString (L"static_mesh::static_mesh graphics_node physics_node model\n");

    name = graphics_node.name;

    opaque_graphics_primitives = std::make_unique<static_graphics_primitives> (graphics_node, MATERIAL_ALPHA_MODE::OPAQUE, model);
    alpha_graphics_primitives = std::make_unique<static_graphics_primitives> (graphics_node, MATERIAL_ALPHA_MODE::ALPHA, model);
    blend_graphics_primitives = std::make_unique<static_graphics_primitives> (graphics_node, MATERIAL_ALPHA_MODE::BLEND, model);
}
