#pragma once

#include "Err.hpp"
#include "Image.hpp"
#include "StaticMesh.hpp"
#include "Background.hpp"

#include <memory>
#include <string>
#include <vector>

class Scene
{
public:
    void Start ();
    void Stop ();

    static std::unique_ptr<Image> ImportImage (std::string AssetName);
    static std::unique_ptr<Background> ImportBackground (std::string AssetName);
    static std::unique_ptr<StaticMesh> ImportStaticMesh (std::string AssetName);

    void CommitAssets ();

private:
    std::vector<std::unique_ptr<Image>> Images;
    std::vector<std::unique_ptr<StaticMesh>> StaticMeshes;
    std::vector<std::unique_ptr<Background>> Backgrounds;
};