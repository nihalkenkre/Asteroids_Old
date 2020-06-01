#include "Game.hpp"
#include "TestScene.hpp"

Game::Game ()
{
    OutputDebugString (L"Game::Game\n");
}

Game::~Game ()
{
    OutputDebugString (L"Game::~Game\n");
}

void Game::ProcessKeyboardInput (WPARAM WParam, LPARAM LParam)
{
    OutputDebugString (L"Game::ProcessKeyboardInput\n");
}

void Game::Init ()
{
    OutputDebugString (L"Game::Init\n");
    
    CurrentScene = new TestScene ();
    CurrentScene->Init ();
}

void Game::MainLoop ()
{
}

void Game::Shutdown ()
{
    OutputDebugString (L"Game::Shutdown\n");
    
    CurrentScene->Shutdown ();
    delete CurrentScene;
}
