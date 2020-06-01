#include "Scene.hpp"
#include "Utils.hpp"

#include <Windows.h>

Scene::Scene ()
{
    OutputDebugString (L"Scene::Scene\n");
}

Scene::~Scene ()
{
    OutputDebugString (L"Scene::~Scene\n");
}

void Scene::ProcessKeyboardInput (WPARAM WParam, LPARAM LParam)
{
}
