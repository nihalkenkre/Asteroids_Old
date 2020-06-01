#include "TestScene.hpp"

TestScene::TestScene ()
{
}

TestScene::~TestScene ()
{
}

void TestScene::Init ()
{
    OutputDebugString (L"TestScene::Init\n");

    ImportMesh ("PlayerShip");
    ImportMesh ("LargeAsteroid");
    ImportMesh ("SmallAsteroid");

    CommitAssets ();
}

void TestScene::ProcessKeyboardInput (WPARAM WParam, LPARAM LParam)
{
    OutputDebugString (L"TestScene::ProcessKeyboardInput\n");
}

void TestScene::MainLoop ()
{
}

void TestScene::Shutdown ()
{
    OutputDebugString (L"TestScene::Shutdown\n");
}
