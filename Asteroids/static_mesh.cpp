#include "static_mesh.hpp"
#include <Windows.h>

static_mesh::static_mesh (const static_mesh& other)
{
    OutputDebugString (L"static_mesh::static_mesh Copy Constructor\n");
    this->name = other.name;
    this->opaque_graphics_primitives->primitives = other.opaque_graphics_primitives->primitives;
}

static_mesh& static_mesh::operator= (const static_mesh& other)
{
    OutputDebugString (L"static_mesh::static_mesh Copy Assignment\n");
    this->name = other.name;
    this->opaque_graphics_primitives->primitives = other.opaque_graphics_primitives->primitives;

    return *this;
}

static_mesh::static_mesh (const static_mesh&& other) noexcept
{
    OutputDebugString (L"static_mesh::static_mesh Move Constructor\n");
    this->name = other.name;
    opaque_graphics_primitives->primitives = std::move (other.opaque_graphics_primitives->primitives);
}

static_mesh& static_mesh::operator= (const static_mesh&& other) noexcept
{
    OutputDebugString (L"static_mesh::static_mesh Move Assignment\n");
    this->name = other.name;
    opaque_graphics_primitives->primitives = std::move (other.opaque_graphics_primitives->primitives);

    return *this;
}

static_mesh::static_mesh (const tinygltf::Node& graphics_node, const std::vector<tinygltf::Node>& physics_nodes, const tinygltf::Model& model)
{
    OutputDebugString (L"static_mesh::static_mesh\n");

    name = graphics_node.name;
}

static_mesh::~static_mesh ()
{
    OutputDebugString (L"static_mesh::~static_mesh\n");
}
