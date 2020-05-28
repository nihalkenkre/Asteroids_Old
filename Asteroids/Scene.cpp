#include "Scene.hpp"

ERR Scene::Start ()
{
    Backgrounds.push_back (Scene::ImportBackground ("Background"));
    Backgrounds.push_back (Scene::ImportBackground ("Background"));
    Backgrounds.push_back (Scene::ImportBackground ("Background"));
    Backgrounds.push_back (Scene::ImportBackground ("Background"));

    Images.push_back (Scene::ImportImage ("Image"));
    Images.push_back (Scene::ImportImage ("Image"));
    Images.push_back (Scene::ImportImage ("Image"));
    Images.push_back (Scene::ImportImage ("Image"));

 
    return ERR::SUCCESS;
}

ERR Scene::Stop ()
{
    return ERR::SUCCESS;
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

ERR Scene::CommitAssets ()
{
    return ERR::SUCCESS;
}
