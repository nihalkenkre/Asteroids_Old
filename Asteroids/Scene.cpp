#include "Scene.hpp"
#include "Utils.hpp"

#include <Windows.h>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>

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
}

void Scene::ProcessKeyboardInput (WPARAM WParam, LPARAM LParam)
{
}

void Scene::MainLoop ()
{
}

void Scene::Shutdown ()
{
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
    for (const auto& ImageName : ImageNames)
    {
    }
}
