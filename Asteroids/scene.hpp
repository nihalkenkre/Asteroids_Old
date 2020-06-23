#pragma once

#include "scene_physics.hpp"
#include "scene_graphics.hpp"
#include "static_meshes.hpp"

#include <Windows.h>
#include <string>
#include <vector>
#include <memory>

class game;
class common_graphics;

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
    scene (const game* obj, const common_graphics* graphics);

    virtual void process_keyboard_input (WPARAM WParam, LPARAM LParam);
    virtual void main_loop ();

protected:
    std::unique_ptr<scene_physics> physics;
    std::unique_ptr<scene_graphics> graphics;

    std::unique_ptr<static_meshes> scene_static_meshes;

    const game* game_obj;
};
