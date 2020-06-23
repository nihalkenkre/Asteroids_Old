#pragma once

#include <Windows.h>

#include "scene.hpp"
#include "common_graphics.hpp"

class game
{
public:
    game (const HINSTANCE& h_instance, const HWND& h_wnd);

    void set_current_scene (SCENE_TYPE type);

    void process_keyboard_input (WPARAM WParam, LPARAM LParam);
    void main_loop ();

private:
    std::unique_ptr<common_graphics> graphics;
    std::unique_ptr<scene> current_scene;
};