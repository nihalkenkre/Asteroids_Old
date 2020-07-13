#include "game.hpp"
#include "test_scene.hpp"

game::game (const HINSTANCE& h_instance, const HWND& h_wnd)
{
    //OutputDebugString (L"game::game h_instance h_wnd\n");

    graphics = std::make_unique<common_graphics> (h_instance, h_wnd);
    set_current_scene (SCENE_TYPE::TEST_SCENE);
}

void game::set_current_scene (SCENE_TYPE type)
{
    switch (type)
    {
        case SCENE_TYPE::TEST_SCENE:
            current_scene.reset ();
            current_scene = std::make_unique<test_scene> (this, graphics.get ());
            break;

        default:
            break;
    }
}

void game::process_keyboard_input (WPARAM WParam, LPARAM LParam)
{
    //OutputDebugString (L"game::process_keyboard_input\n");

    current_scene->process_keyboard_input (WParam, LParam);
}

void game::main_loop ()
{
    current_scene->main_loop ();
}
