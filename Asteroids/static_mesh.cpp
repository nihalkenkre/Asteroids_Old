#include "static_mesh.hpp"
#include <Windows.h>


static_mesh::static_mesh (const tinygltf::Node& graphics_node, const std::vector<tinygltf::Node>& physics_nodes, const tinygltf::Model& model)
{
    OutputDebugString (L"static_mesh::static_mesh\n");

    name = graphics_node.name;
}

static_mesh::~static_mesh ()
{
    OutputDebugString (L"static_mesh::~static_mesh\n");
}
