#pragma once

#include <Windows.h>

class Scene
{
public:
    Scene ();
    ~Scene ();

    void ProcessKeyboardInput (WPARAM WParam, LPARAM LParam);
};
