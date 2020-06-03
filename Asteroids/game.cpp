#include "game.hpp"
#include "test_scene.hpp"


game::game ()
{
    OutputDebugString (L"game::game\n");
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

void game::init ()
{
    OutputDebugString (L"game::init\n");
    
    current_scene = std::make_unique<test_scene> ();
    current_scene->init ();
}

void game::main_loop ()
{
}

void game::shutdown ()
{
    OutputDebugString (L"game::shutdown\n");
    
    current_scene->shutdown ();
}
