#include "vk_asset.hpp"

vk_image::vk_image ()
{
    image = {};
    image_view = {};
}

vk_image::vk_image (vk::Image image_, vk::ImageView image_view_)
{
    image = image_;
    image_view = image_view_;
}

vk_image::~vk_image ()
{
}
