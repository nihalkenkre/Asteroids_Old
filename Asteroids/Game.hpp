#pragma once

#include <Windows.h>

#include "Err.hpp"

class Scene;

class Game
{
public:
    Game ();
    ~Game ();

    void Init ();
    void ProcessKeyboardInput (WPARAM WParam, LPARAM LParam);
    void MainLoop ();
    void Shutdown ();

private:
    Scene* CurrentScene = nullptr;
};