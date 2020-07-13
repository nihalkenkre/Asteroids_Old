#include "test_scene.hpp"


test_scene::test_scene (const game* obj, const common_graphics* c_graphics) : scene (obj, c_graphics)
{
    //OutputDebugString (L"test_scene::test_scene\n");
}

test_scene::~test_scene ()
{
    //OutputDebugString (L"test_scene::~test_scene\n");
}

void test_scene::process_keyboard_input (WPARAM w_param, LPARAM l_param)
{
    //OutputDebugString (L"test_scene::process_keyboard_input\n");
}

void test_scene::main_loop ()
{
    graphics->main_loop ();
}
