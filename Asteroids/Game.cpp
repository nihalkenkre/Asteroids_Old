#include "Game.hpp"

#include "FirstScene.hpp"
#include "SecondScene.hpp"

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
