#include "scene_assets.hpp"

#include <Windows.h>

scene_assets::scene_assets (const gltf_models& models, const std::vector<std::string>& search_names)
{
    OutputDebugString (L"scene_assets::scene_assets models search_names\n");

    std::vector<tinygltf::Model> mods;
    mods.reserve (5);
    std::vector<tinygltf::Node> graphics_nodes;
    graphics_nodes.reserve (5);
    std::vector<std::vector<tinygltf::Node>> physics_nodes;

    static_meshes.reserve (5);

    for (const auto& current_model : models.models)
    {
        for (const auto& current_node : current_model.nodes)
        {
            if (current_node.skin > -1)
            {
                continue;
            }

            if (current_node.mesh < 0)
            {
                continue;
            }

            if (current_node.name.find ("CS_") != std::string::npos)
            {
                continue;
            }

            for (const auto& search_name : search_names)
            {
                if (current_node.name.find (search_name) == std::string::npos)
                {
                    continue;
                }

                graphics_nodes.push_back (current_node);
                mods.push_back (current_model);
            }
        }
    }

    physics_nodes.resize (graphics_nodes.size ());

    for (const auto& current_mod : mods)
    {
        for (const auto& current_node : current_mod.nodes)
        {
            if (current_node.skin > -1)
            {
                continue;
            }

            if (current_node.mesh < 0)
            {
                continue;
            }

            if (current_node.name.find ("CS_") == std::string::npos)
            {
                continue;
            }

            for (const auto& search_name : search_names)
            {
                if (current_node.name.find (search_name) == std::string::npos)
                {
                    continue;
                }
                for (size_t g = 0;g < graphics_nodes.size (); ++g)
                {
                    if (current_node.name.find (graphics_nodes.at (g).name) != std::string::npos)
                    {
                        physics_nodes.at (g).push_back (current_node);
                    }
                }
            }
        }
    }

    for (size_t g = 0; g < graphics_nodes.size (); ++g)
    {
        static_meshes.emplace_back (static_mesh (graphics_nodes.at (g), physics_nodes.at (g), mods.at (g)));
    }
}