#pragma once

#include <Windows.h>
#include <memory>

#include "err.hpp"
#include "common_graphics.hpp"


class scene;


class game
{
public:
    game ();
    ~game ();

    void init (HINSTANCE h_instance, HWND h_wnd);
    void process_keyboard_input (WPARAM WParam, LPARAM LParam);
    void main_loop ();

private:
    std::unique_ptr<common_graphics> graphics = std::make_unique<common_graphics> ();
    std::unique_ptr<scene> current_scene = nullptr;
};