#pragma once

#include <Windows.h>
#include <memory>

#include "Err.hpp"

class Scene;

class Game
{
public:
    Game ();
    ~Game ();

    void ProcessKeyboardInput (WPARAM WParam, LPARAM LParam);

    void Init ();
    void MainLoop ();
    void Shutdown ();

private:
    Scene* CurrentScene;
};