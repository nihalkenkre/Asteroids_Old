#include "scene.hpp"
#include "scene_graphics.hpp"
#include "scene_physics.hpp"

#include <Windows.h>


scene::scene (game* obj)
{
    OutputDebugString (L"scene::scene\n");
    
    physics = std::make_unique<scene_physics> ();
    graphics = std::make_unique<scene_graphics> ();

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