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

    void Init ();
    void ProcessKeyboardInput (WPARAM WParam, LPARAM LParam);
    void MainLoop ();
    void Shutdown ();

private:
    std::unique_ptr<Scene> CurrentScene = nullptr;
};