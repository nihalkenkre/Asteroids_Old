#include "Game.hpp"
#include "Scene.hpp"

void Game::Start ()
{
    CurrentScene = Game::CreateScene <Scene> ();
    
    CurrentScene->Start ();
}

void Game::ProcessKeyboardInput (WPARAM WParam, LPARAM LParam)
{
}

void Game::Stop ()
{
    CurrentScene->Stop ();
}