#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <memory>

#include "scene_physics.hpp"
#include "scene_graphics.hpp"
#include "static_meshes.hpp"

class game;


enum class SCENE_TYPE
{
    SPLASH_SCENE,
    TEST_SCENE,
    MAIN_SCENE,
    MENU_SCENE
};


class scene
{
public:
    scene ();
    scene (game* obj);
    virtual ~scene ();

    virtual void process_keyboard_input (WPARAM WParam, LPARAM LParam);
    virtual void main_loop ();

private:
    scene_physics physics;
    scene_graphics graphics;

    static_meshes scene_static_meshes;

    game* game_obj;
};
