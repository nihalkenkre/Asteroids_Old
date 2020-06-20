#include "scene.hpp"
#include "scene_graphics.hpp"
#include "scene_physics.hpp"
#include "gltf_models.hpp"
#include "static_meshes.hpp"

#include <Windows.h>


scene::scene ()
{
    OutputDebugString (L"scene::scene\n");
}

scene::scene (game* obj)
{
    OutputDebugString (L"scene::scene game\n");
    
    gltf_models models ("");

    scene_static_meshes = static_meshes (models, std::vector<std::string> {"PlayerShip", "LargeAsteroid"});

    physics = scene_physics ();
    graphics = scene_graphics ();

    this->game_obj = obj;
}

scene::~scene ()
{
    OutputDebugString (L"scene::~scene\n");
}

void scene::process_keyboard_input (WPARAM WParam, LPARAM LParam)
{
    OutputDebugString (L"scene::process_keyboard_input\n");
}

void scene::main_loop ()
{
}