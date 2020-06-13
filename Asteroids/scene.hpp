#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <memory>


class game;
class scene_graphics;
class scene_physics;


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
    scene (game* obj);
    virtual ~scene ();

    virtual void process_keyboard_input (WPARAM WParam, LPARAM LParam);
    virtual void main_loop ();

private:
    std::unique_ptr<scene_physics> physics = nullptr;
    std::unique_ptr<scene_graphics> graphics = nullptr;

    game* game_obj;
};
