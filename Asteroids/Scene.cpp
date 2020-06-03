#include "Scene.hpp"
#include "SceneGraphics.hpp"

#include <Windows.h>


Scene::Scene ()
{
    OutputDebugString (L"Scene::Scene\n");
}

Scene::~Scene ()
{
    OutputDebugString (L"Scene::~Scene\n");
}

void Scene::Init ()
{
    OutputDebugString (L"Scene::Init\n");
}

void Scene::ProcessKeyboardInput (WPARAM WParam, LPARAM LParam)
{
    OutputDebugString (L"Scene::ProcessKeyboardInput\n");
}

void Scene::MainLoop ()
{
}

void Scene::Shutdown ()
{
    OutputDebugString (L"Scene::Shutdown\n");
}

void Scene::ImportImage (std::string Name)
{
    ImageNames.push_back (Name.append (".tga"));
}

void Scene::ImportMesh (std::string Name)
{
    MeshNames.push_back (Name.append (".glb"));
}

void Scene::CommitAssets ()
{
    Graphics->CreateGraphicsForMeshes (MeshNames);
    Graphics->CreateGraphicsForImages (ImageNames);
}
