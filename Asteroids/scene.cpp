#include "scene.hpp"
#include "scene_graphics.hpp"

#include <Windows.h>


scene::scene ()
{
    OutputDebugString (L"scene::scene\n");
    graphics = std::make_unique<scene_graphics> ();
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