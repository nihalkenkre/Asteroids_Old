#pragma once

#include "scene.hpp"


class game;
class common_graphics;


class test_scene : public scene
{
public:
    test_scene (const game* obj, const common_graphics* c_graphics);
    ~test_scene ();

    void process_keyboard_input (WPARAM WParam, LPARAM LParam) override;
    void main_loop () override;
};