#pragma once

#include "scene.hpp"


class game;


class test_scene : public scene
{
public:
    test_scene (game* obj);
    ~test_scene ();

    void process_keyboard_input (WPARAM WParam, LPARAM LParam) override;
    void main_loop () override;

private:
    game* game_obj;
};