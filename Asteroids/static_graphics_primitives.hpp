#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>
#include <tiny_gltf.h>

class static_graphics_primitive
{
public:
    static_graphics_primitive (const tinygltf::Primitive& primitive, const tinygltf::Model& model);

    std::vector<unsigned char> positions;
    std::vector<unsigned char> normals;
    std::vector<unsigned char> uv0s;
    std::vector<unsigned char> indices;

    vk::DeviceSize positions_offset;
    vk::DeviceSize normals_offset;
    vk::DeviceSize uv0s_offset;
    vk::DeviceSize indices_offset;

    size_t index_count;
    vk::IndexType index_type;
};


class static_graphics_primitives
{
public:
    static_graphics_primitives (const tinygltf::Node& graphics_node, const tinygltf::Model& model);
    
    std::vector<static_graphics_primitive> primitives;
};