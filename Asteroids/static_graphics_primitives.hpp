#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>
#include <tiny_gltf.h>

class static_graphics_primitive
{
public:
    static_graphics_primitive ();
    static_graphics_primitive (const tinygltf::Primitive& primitive, const tinygltf::Model& model);
    static_graphics_primitive (const static_graphics_primitive& other);
    static_graphics_primitive& operator= (const static_graphics_primitive& other);
    static_graphics_primitive (const static_graphics_primitive&& other) noexcept;
    static_graphics_primitive& operator= (const static_graphics_primitive&& other) noexcept;
    ~static_graphics_primitive () noexcept;

    std::vector<unsigned char> positions;
    std::vector<unsigned char> normals;
    std::vector<unsigned char> uv0s;
    std::vector<unsigned char> uv1s;
    std::vector<unsigned char> indices;

    vk::DeviceSize positions_offset;
    vk::DeviceSize normals_offset;
    vk::DeviceSize uv0s_offset;
    vk::DeviceSize uv1s_offset;
    vk::DeviceSize indices_offset;

    size_t index_count;
    vk::IndexType index_type;
};


class static_graphics_primitives
{
public:
    static_graphics_primitives ();
    static_graphics_primitives (const tinygltf::Node& graphics_node, const tinygltf::Model& model);
    static_graphics_primitives (const static_graphics_primitives& other);
    static_graphics_primitives& operator= (const static_graphics_primitives& other);
    static_graphics_primitives (const static_graphics_primitives&& other) noexcept;
    static_graphics_primitives& operator = (const static_graphics_primitives&& other) noexcept;
    ~static_graphics_primitives () noexcept;

    std::vector<static_graphics_primitive> primitives;
};