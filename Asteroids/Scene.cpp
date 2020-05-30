#include "Scene.hpp"

void Scene::Start ()
{
    Backgrounds.push_back (Scene::ImportBackground ("Background"));
    Backgrounds.push_back (Scene::ImportBackground ("Background"));
    Backgrounds.push_back (Scene::ImportBackground ("Background"));
    Backgrounds.push_back (Scene::ImportBackground ("Background"));

    Images.push_back (Scene::ImportImage ("Image"));
    Images.push_back (Scene::ImportImage ("Image"));
    Images.push_back (Scene::ImportImage ("Image"));
    Images.push_back (Scene::ImportImage ("Image"));
}

void Scene::Stop ()
{
}

std::unique_ptr<Image> Scene::ImportImage (std::string AssetName)
{
    return std::unique_ptr<Image> ();
}

std::unique_ptr<Background> Scene::ImportBackground (std::string AssetName)
{
    return std::unique_ptr<Background> ();
}

std::unique_ptr<StaticMesh> Scene::ImportStaticMesh (std::string AssetName)
{
    return std::unique_ptr<StaticMesh> ();
}

void Scene::CommitAssets ()
{
}
