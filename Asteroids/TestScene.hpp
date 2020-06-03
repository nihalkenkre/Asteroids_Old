#pragma once

#include "Scene.hpp"


class TestScene : public Scene
{
public:
    TestScene ();
    ~TestScene ();

    void Init () override;
    void ProcessKeyboardInput (WPARAM WParam, LPARAM LParam) override;
    void MainLoop () override;
    void Shutdown () override;
};