#include "vk_image.hpp"

vk_image::vk_image (const tinygltf::Image& image)
{
    name = image.name;
    image_data = image.image;
    width = image.width;
    height = image.height;
}

vk_image::~vk_image ()
{

}