#pragma once

#include <Windows.h>
#include <memory>

#include "err.hpp"
#include "common_graphics.hpp"


class scene;


class game
{
public:
    game (HINSTANCE h_instance, HWND h_wnd);
    ~game ();

    void process_keyboard_input (WPARAM WParam, LPARAM LParam);
    void main_loop ();

private:
    std::unique_ptr<common_graphics> graphics = nullptr;
    std::unique_ptr<scene> current_scene = nullptr;
};