#include "Game.hpp"
#include "Scene.hpp"

ERR Game::Start ()
{
    CurrentScene = Game::CreateScene <Scene> ();
    
    ERR Result = ERR::SUCCESS;

    CHECK_ERR (CurrentScene->Start (), Result);
    
    return ERR::SUCCESS;
}

ERR Game::ProcessKeyboardInput (WPARAM WParam, LPARAM LParam)
{
    return ERR::SUCCESS;
}

void Game::Stop ()
{
    CurrentScene->Stop ();
}