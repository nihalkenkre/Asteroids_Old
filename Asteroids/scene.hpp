#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <memory>


class game;
class scene_graphics;
class scene_physics;
class static_meshes;


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

    std::unique_ptr<static_meshes> scene_static_meshes = nullptr;

    game* game_obj;
};
