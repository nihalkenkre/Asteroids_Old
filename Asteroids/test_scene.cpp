#include "test_scene.hpp"


void test_scene::init ()
{
    OutputDebugString (L"test_scene::init\n");

    import_mesh ("PlayerShip");
    import_mesh ("LargeAsteroid");
    import_mesh ("SmallAsteroid");

    commit_assets ();
}

void test_scene::process_keyboard_input (WPARAM WParam, LPARAM LParam)
{
    OutputDebugString (L"test_scene::process_keyboard_input\n");
}

void test_scene::main_loop ()
{
}

void test_scene::shutdown ()
{
    OutputDebugString (L"test_scene::shutdown\n");
}
