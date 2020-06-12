#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <memory>

#include "err.hpp"


class scene_graphics;


class scene
{
public:
    scene ();
    ~scene ();

    virtual void process_keyboard_input (WPARAM WParam, LPARAM LParam);
    virtual void main_loop ();

protected:

private:

    std::unique_ptr<scene_graphics> graphics = nullptr;
};
