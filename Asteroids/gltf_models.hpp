#pragma once

#include <string>
#include <tiny_gltf.h>
#include <vector>

class vector;

class gltf_models
{
public:
    gltf_models (const std::string& folder_path);
    ~gltf_models ();

    std::vector<tinygltf::Model> models;
};