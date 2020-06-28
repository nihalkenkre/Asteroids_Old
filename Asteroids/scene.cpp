#include "scene.hpp"
#include "scene_graphics.hpp"
#include "scene_physics.hpp"
#include "gltf_models.hpp"
#include "common_graphics.hpp"

#include <Windows.h>


scene::scene (const game* obj, const common_graphics* c_graphics) : game_obj (obj)
{
    OutputDebugString (L"scene::scene game\n");

    gltf_models models ("");

    assets = std::make_unique<scene_assets> (models, std::vector<std::string> {"PlayerShip", "LargeAsteroid"});

    physics = std::make_unique<scene_physics> ();
    graphics = std::make_unique<scene_graphics> (assets.get (), c_graphics);
}


void scene::process_keyboard_input (WPARAM WParam, LPARAM LParam)
{
    OutputDebugString (L"scene::process_keyboard_input\n");
}

void scene::main_loop ()
{
}