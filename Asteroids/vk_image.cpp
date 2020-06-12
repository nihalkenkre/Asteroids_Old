#include "vk_image.hpp"

vk_texture::vk_texture (const tinygltf::Image& image)
{
    name = image.name;
    image_data = image.image;
    width = image.width;
    height = image.height;
}

vk_texture::~vk_texture ()
{

}