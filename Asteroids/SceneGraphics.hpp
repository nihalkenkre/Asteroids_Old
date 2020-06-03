#pragma once

#include <vulkan/vulkan.hpp>


class SceneGraphics
{
public:
    SceneGraphics ();
    ~SceneGraphics ();

    void CreateGraphicsForMeshes (std::vector<std::string> FilePaths);
    void CreateGraphicsForImages (std::vector<std::string> FilePaths);

private:
    void ImportGLTFs (std::vector<std::string> FilePaths);
    void ImportImages (std::vector<std::string> FilePaths);

    vk::Buffer VertexIndexBuffer;
    vk::DeviceMemory VertexIndexBufferMemory;
};