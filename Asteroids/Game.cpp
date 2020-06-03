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
    CurrentScene->ProcessKeyboardInput (WParam, LParam);
}

void Game::Init ()
{
    OutputDebugString (L"Game::Init\n");
    
    CurrentScene = std::make_unique<TestScene> ();
    CurrentScene->Init ();
}

void Game::MainLoop ()
{
}

void Game::Shutdown ()
{
    OutputDebugString (L"Game::Shutdown\n");
    
    CurrentScene->Shutdown ();
}
