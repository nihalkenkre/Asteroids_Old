#include "image.hpp"


image::image (const tinygltf::Image& image, const tinygltf::Model& model) : name (image.name), width (image.width), height (image.height), image_data (image.image)
{
    OutputDebugString (L"image::image image, model");
}
