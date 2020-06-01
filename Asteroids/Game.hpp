#pragma once

#include <Windows.h>

#include <thread>

class Game
{
public:
    Game ();
    ~Game ();

    void ProcessKeyboardInput (WPARAM WParam, LPARAM LParam);

private:
};