#include "test_scene.hpp"


test_scene::test_scene (game* obj) : scene (obj)
{
    OutputDebugString (L"test_scene::test_scene\n");
    
    this->game_obj = obj;
}

test_scene::~test_scene ()
{
    OutputDebugString (L"test_scene::~test_scene\n");
}

void test_scene::process_keyboard_input (WPARAM WParam, LPARAM LParam)
{
    OutputDebugString (L"test_scene::process_keyboard_input\n");
}

void test_scene::main_loop ()
{
}
