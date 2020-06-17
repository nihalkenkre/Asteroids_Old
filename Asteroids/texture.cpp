#include "textures.hpp"

texture::texture (const texture& other)
{
    OutputDebugString (L"texture::texture Copy constructor\n");

    *this = other;
}

texture& texture::operator=(const texture& other)
{
    OutputDebugString (L"texture::texture Copy assignment\n");

    name = other.name;
    image_data = other.image_data;
    width = other.width;
    height = other.height;
    image_data_offset = other.image_data_offset;
    image = other.image;
    image_view = other.image_view;

    return *this;
}

texture::texture (texture&& other) noexcept
{
    OutputDebugString (L"texture::texture Move constructor\n");

    *this = std::move (other);
}

texture& texture::operator=(texture&& other) noexcept
{
    name = other.name;
    width = other.width;
    height = other.height;
    image_data_offset = other.image_data_offset;

    image_data = std::move (other.image_data);
    image = std::move (other.image);
    image_view = std::move (other.image_view);

    other.name.clear ();
    other.height = 0;
    other.width = 0;
    other.image_data_offset = 0;

    return *this;
}

texture::texture (const tinygltf::Image& image) : name (image.name), image_data (image.image), width(image.width), height (image.height)
{
}

texture::~texture () noexcept
{

}