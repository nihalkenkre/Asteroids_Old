#include "static_meshes.hpp"
#include "gltf_models.hpp"

#include <Windows.h>


static_mesh::static_mesh (const tinygltf::Node& graphics_node, const std::vector<tinygltf::Node>& physics_nodes, const tinygltf::Model& model)
{
    OutputDebugString (L"static_mesh::static_mesh\n");

    
}

static_mesh::~static_mesh ()
{
    OutputDebugString (L"static_mesh::~static_mesh\n");
}


static_meshes::static_meshes (const gltf_models& models, const std::vector<std::string>& search_names)
{
    OutputDebugString (L"static_meshes::static_meshes\n");

    meshes.reserve (5);

    for (const auto& model : models.models)
    {
        for (const auto& search_name : search_names)
        {
            std::vector<tinygltf::Node> physics_nodes;
            physics_nodes.reserve (5);
            tinygltf::Node graphics_node;

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

                if (node.name.find (search_name) != std::string::npos)
                {
                    if (node.name.find ("CS_") == std::string::npos)
                    {
                        physics_nodes.push_back (node);
                    }
                    else
                    {
                        graphics_node = node;
                    }
                    meshes.emplace_back (static_mesh (graphics_node, physics_nodes, model));
                }
            }
        }
    }
}

static_meshes::~static_meshes ()
{
    OutputDebugString (L"static_meshes::~static_meshes\n");
}