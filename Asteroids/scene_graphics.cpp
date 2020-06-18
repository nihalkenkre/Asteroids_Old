#include "scene_graphics.hpp"
#include "common_graphics.hpp"
#include "utils.hpp"
#include "vk_utils.hpp"


scene_graphics::scene_graphics ()
{
}

scene_graphics::scene_graphics (static_meshes* meshes)
{
    OutputDebugString (L"scene_graphics::scene_graphics meshes\n");

}

scene_graphics::~scene_graphics ()
{
    OutputDebugString (L"scene_graphics::~scene_graphics\n");
}