#include "scene.hpp"
#include "scene_graphics.hpp"

#include <Windows.h>


scene::scene ()
{
    OutputDebugString (L"scene::scene\n");
}

scene::~scene ()
{
    OutputDebugString (L"scene::~scene\n");
}

void scene::init ()
{
    OutputDebugString (L"scene::init\n");
}

void scene::process_keyboard_input (WPARAM WParam, LPARAM LParam)
{
    OutputDebugString (L"scene::process_keyboard_input\n");
}

void scene::main_loop ()
{
}

void scene::import_image (std::string Name)
{
    image_names.push_back (Name.append (".tga"));
}

void scene::import_mesh (std::string Name)
{
    mesh_names.push_back (Name.append (".glb"));
}

void scene::commit_assets ()
{
    graphics->create_graphics_for_meshes (mesh_names);
    graphics->create_graphics_for_images (image_names);
}
