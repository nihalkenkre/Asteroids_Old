#pragma once

#include <string>
#include <vector>

#include <tiny_gltf.h>


class vector;


class gltf_models
{
public:
    gltf_models (const std::string& folder_path);
    ~gltf_models ();

    std::vector<tinygltf::Model> models;
};