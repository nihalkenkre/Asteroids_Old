#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <memory>

#include "Err.hpp"


class SceneGraphics;


class Scene
{
public:
    Scene ();
    ~Scene ();

    virtual void Init ();
    virtual void ProcessKeyboardInput (WPARAM WParam, LPARAM LParam);
    virtual void MainLoop ();
    virtual void Shutdown ();

protected:
    void ImportImage (std::string Name);
    void ImportMesh (std::string Name);
    
    void CommitAssets ();

private:
    std::vector <std::string> ImageNames;
    std::vector <std::string> MeshNames;

    std::unique_ptr<SceneGraphics> Graphics = std::make_unique<SceneGraphics> ();
};
