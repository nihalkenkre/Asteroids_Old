#pragma once

#include <string>
#include <vector>

#include <tiny_gltf.h>


class gltf_models
{
public:
    gltf_models ();
    gltf_models (const gltf_models& other);
    gltf_models& operator= (const gltf_models& other);
    gltf_models (gltf_models&& other) noexcept;
    gltf_models& operator= (gltf_models&& other) noexcept;
    gltf_models (const std::string& folder_path);
    ~gltf_models () noexcept;

    std::vector<tinygltf::Model> models;
};