#include "static_mesh.hpp"
#include "gltf_models.hpp"

#include <Windows.h>


static_mesh::static_mesh (const tinygltf::Node& node, const tinygltf::Model& model)
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
        for (const auto& node : model.nodes)
        {
            if (node.name.find ("CS_") != std::string::npos)
            {
                continue;
            }

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
                if (node.name.compare (search_name) == 0)
                {
                    meshes.push_back (std::make_unique <static_mesh> (node, model));
                    break;
                }
            }
        }
    }
}

static_meshes::~static_meshes ()
{
    OutputDebugString (L"static_meshes::~static_meshes\n");
}