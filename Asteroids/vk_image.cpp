#include "vk_image.hpp"

texture::texture (const tinygltf::Image& image)
{
    name = image.name;
    image_data = image.image;
    width = image.width;
    height = image.height;
}

texture::~texture ()
{

}