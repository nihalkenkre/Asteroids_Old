#pragma once

#include "vk_image.hpp"

#include <string>
#include <memory>
#include <vulkan/vulkan.hpp>

/*class vk_texture
{
public:
    vk_texture ();
    ~vk_texture ();

    std::string name;

    std::vector<unsigned char> image_data;
    vk::DeviceSize image_data_offset;

    vk::DeviceSize width;
    vk::DeviceSize height;

    vk::Image image;
    vk::ImageView image_view;
};*/

class vk_animation
{
public:
    std::string name;

    std::vector<vk::DeviceSize> frame_data_offsets;
};

#undef OPAQUE // disable wingdi background definition enum - we are not using it at all
enum class MATERIAL_ALPHA_MODE
{
    OPAQUE,
    MASK,
    BLEND,
};

class vk_material
{
public:
    std::string name;

    vk_texture base_texture;
    vk_texture metalness_roughness_texture;
    vk_texture normal_texture;
    vk_texture occlusion_texture;
    vk_texture emissive_texture;

    std::array<float, 4> base_color_factor;
    float metalness_factor;
    float roughness_factor;
    std::array<float, 3> emissive_factor;

    MATERIAL_ALPHA_MODE alpha_mode;
};

class vk_static_primitive
{
public:
    std::string name;

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

    vk::DeviceSize num_indices;
    vk::IndexType index_type;

    vk_material material;
};

class vk_static_mesh
{
public:
    std::string name;

    std::vector<vk_static_primitive> opaque_primitives;
    std::vector<vk_static_primitive> alpha_primitives;
    std::vector<vk_static_primitive> blend_primitives;
};