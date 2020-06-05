#pragma once

#include <string>
#include <vulkan/vulkan.hpp>

class vk_image
{
public:
    vk_image ();
    vk_image (vk::Image* image_, vk::ImageView* image_view_);

    ~vk_image ();

    std::string name;

    vk::Image* image;
    vk::ImageView* image_view;
};

class vk_animation
{
public:
    std::string name;

    std::vector<vk::DeviceSize> frame_data_offsets;
};

#undef OPAQUE // disable wingdi background definition - we are not using it at all
enum class MATERIAL_ALPHA_MODE
{
    OPAQUE,
    MASK,
    BLEND,
};

class vk_material
{
    std::string name;

    vk_image base_texture;
    vk_image metalness_roughness_texture;
    vk_image normal_texture;
    vk_image occlusion_texture;
    vk_image emissive_texture;

    std::array<float, 4> base_color_factor;
    float metalness_factor;
    float roughness_factor;
    std::array<float, 3> emissive_factor;

    MATERIAL_ALPHA_MODE alpha_mode;
};

class vk_skinned_primitive
{
public:

};