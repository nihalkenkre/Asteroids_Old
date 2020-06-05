#pragma once

#include "vk_asset.hpp"
#include <vector>


class asset_io
{
public:
    static std::vector<vk_image> import_images (const std::string& file_path);

private:
};