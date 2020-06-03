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

    void init ();
    void process_keyboard_input (WPARAM WParam, LPARAM LParam);
    void main_loop ();
    void shutdown ();

private:
    std::unique_ptr<scene> current_scene = nullptr;
    std::unique_ptr<common_graphics> graphics = std::make_unique<common_graphics> ();
};