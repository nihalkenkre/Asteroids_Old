#pragma once

#include "vk_asset.hpp"

#include <vulkan/vulkan.hpp>
#include <tiny_gltf.h>


class static_meshes;


class scene_graphics
{
public:
    scene_graphics ();
    scene_graphics (static_meshes* meshes);
    ~scene_graphics () noexcept;

private:
};