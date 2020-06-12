#include "game.hpp"
#include "test_scene.hpp"


game::game (HINSTANCE h_instance, HWND h_wnd)
{
    OutputDebugString (L"game::game\n");

    graphics = std::make_unique<common_graphics> (h_instance, h_wnd);
}

game::~game ()
{
    OutputDebugString (L"game::~game\n");
}

void game::process_keyboard_input (WPARAM WParam, LPARAM LParam)
{
    OutputDebugString (L"game::process_keyboard_input\n");
    current_scene->process_keyboard_input (WParam, LParam);
}

void game::main_loop ()
{
}
