#include "static_meshes.hpp"
#include "gltf_models.hpp"

#include <tiny_gltf.h>

#include <map>
#include <Windows.h>


static_mesh::static_mesh (const static_mesh& other)
{
    OutputDebugString (L"static_mesh::static_mesh Copy constructor\n");

    *this = other;
}

static_mesh& static_mesh::operator= (const static_mesh& other)
{
    OutputDebugString (L"static_mesh::static_mesh Copy assignment\n");

    name = other.name;
    opaque_graphics_primitives = other.opaque_graphics_primitives;
    alpha_graphics_primitives = other.alpha_graphics_primitives;
    blend_graphics_primitives = other.blend_graphics_primitives;

    return *this;
}

static_mesh::static_mesh (static_mesh&& other) noexcept
{
    OutputDebugString (L"static_mesh::static_mesh Move constructor\n");

    *this = std::move (other);
}

static_mesh& static_mesh::operator= (static_mesh&& other) noexcept
{
    OutputDebugString (L"static_mesh::static_mesh Move assignment\n");

    name = other.name;
    opaque_graphics_primitives = std::move (other.opaque_graphics_primitives);
    alpha_graphics_primitives = std::move (other.alpha_graphics_primitives);
    blend_graphics_primitives = std::move (other.blend_graphics_primitives);

    return *this;
}

static_mesh::static_mesh ()
{
    OutputDebugString (L"static_mesh::static_mesh\n");
}

static_mesh::static_mesh (const tinygltf::Node& graphics_node, const std::vector<tinygltf::Node>& physics_nodes, const tinygltf::Model& model)
{
    OutputDebugString (L"static_mesh::static_mesh graphics_node physics_node model\n");

    name = graphics_node.name;
    opaque_graphics_primitives = static_graphics_primitives (graphics_node, model);
    alpha_graphics_primitives = static_graphics_primitives (graphics_node, model);
    blend_graphics_primitives = static_graphics_primitives (graphics_node, model);

}

static_mesh::~static_mesh ()
{
    OutputDebugString (L"static_mesh::~static_mesh\n");
}



static_meshes::static_meshes (const static_meshes& other)
{
    OutputDebugString (L"static_meshes::static_meshes Copy constructor\n");

    *this = other;
}

static_meshes& static_meshes::operator=(const static_meshes& other)
{
    OutputDebugString (L"static_meshes::static_meshes Copy assignment\n");
    
    meshes = other.meshes;

    return *this;
}

static_meshes::static_meshes (static_meshes&& other) noexcept
{
    OutputDebugString (L"static_meshes::static_meshes Move constructor\n");

    *this = std::move (other);
}

static_meshes& static_meshes::operator=(static_meshes&& other) noexcept
{
    OutputDebugString (L"static_meshes::static_meshes Move Assignment\n");

    meshes = std::move (other.meshes);

    return *this;
}

static_meshes::static_meshes ()
{
    OutputDebugString (L"static_meshes::static_meshes\n");
}

static_meshes::static_meshes (const gltf_models& models, const std::vector<std::string>& search_names)
{
    OutputDebugString (L"static_meshes::static_meshes models search_names\n");

    meshes.reserve (5);

    std::vector<tinygltf::Model> mods;
    mods.reserve (5);    

    std::vector<tinygltf::Node> graphics_nodes;
    graphics_nodes.reserve (5);
    std::vector<std::vector<tinygltf::Node>> physics_nodes;

    for (const auto& model : models.models)
    {
        for (const auto& node : model.nodes)
        {
            if (node.skin > -1)
            {
                continue;
            }

            if (node.mesh < 0)
            {
                continue;
            }

            for (const auto& search_name : search_names)
            {
                if (node.name.find (search_name) != std::string::npos && node.name.find ("CS_") == std::string::npos)
                {
                    graphics_nodes.push_back (node);
                    mods.push_back (model);
                }
            }
        }
    }

    physics_nodes.resize (graphics_nodes.size ());

    for (const auto& model : models.models)
    {
        for (const auto& node : model.nodes)
        {
            if (node.skin > -1)
            {
                continue;
            }

            if (node.mesh < 0)
            {
                continue;
            }

            for (const auto& search_name : search_names)
            {
                for (size_t g = 0; g < graphics_nodes.size (); ++g)
                {
                    if (node.name.find (search_name) != std::string::npos && node.name.find ("CS_") == std::string::npos && node.name.find (graphics_nodes.at(g).name) != std::string::npos)
                    {
                        physics_nodes.at (g).push_back (node);
                    }
                }
            }
        }
    }

    for (size_t g = 0; g < graphics_nodes.size (); ++g)
    {
        meshes.emplace_back (static_mesh (graphics_nodes.at (g), physics_nodes.at (g), mods.at (g)));
    }
}

static_meshes::~static_meshes ()
{
    OutputDebugString (L"static_meshes::~static_meshes\n");
}