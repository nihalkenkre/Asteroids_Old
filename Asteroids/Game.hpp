#pragma once

#include "Err.hpp"
#include "Scene.hpp"
#include <Windows.h>
#include <memory>

class Game
{
public:
    Game () {}
    ~Game () {}

    void Start ();
    void ProcessKeyboardInput (WPARAM WParam, LPARAM LParam);
    void Stop ();

    template<typename T>
    static std::unique_ptr<T> CreateScene ();

private:
    std::unique_ptr<Scene> CurrentScene = nullptr;
};

template<typename T>
inline std::unique_ptr<T> Game::CreateScene ()
{
    return std::make_unique<T> ();
}
