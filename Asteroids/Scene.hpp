#pragma once

#include <Windows.h>
#include <string>
#include <vector>

#include "Err.hpp"

class SceneGraphics;
class ScenePhysics;

class Scene
{
public:
    Scene ();
    ~Scene ();

    virtual void Init ();
    virtual void ProcessKeyboardInput (WPARAM WParam, LPARAM LParam);
    virtual void MainLoop ();
    virtual void Shutdown ();

    void ImportImage (std::string Name);
    void ImportMesh (std::string Name);
    
    void CommitAssets ();

private:
    std::vector <std::string> ImageNames;
    std::vector <std::string> MeshNames;

    SceneGraphics* Graphics = nullptr;
    ScenePhysics* Physics = nullptr;
};
