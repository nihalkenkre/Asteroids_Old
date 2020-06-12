#include "scene_graphics.hpp"
#include "common_graphics.hpp"
#include "utils.hpp"
#include "vk_utils.hpp"
#include "asset_io.hpp"


scene_graphics::scene_graphics ()
{
    OutputDebugString (L"scene_graphics::scene_graphics\n");

    static_meshes.reserve (5);

    //images.reserve (5);
    //image_views.reserve (5);
}

scene_graphics::~scene_graphics ()
{
    OutputDebugString (L"scene_graphics::~scene_graphics\n");

    /*common_graphics::graphics_device->get_obj ().freeMemory (images_memory);

    common_graphics::graphics_device->get_obj ().destroyBuffer (vertex_index_buffer);
    common_graphics::graphics_device->get_obj ().freeMemory (vertex_index_buffer_memory);*/
}

void scene_graphics::create_graphics_for_meshes (const std::vector<std::string>& file_paths)
{
    asset_io importer (file_paths);
    images = importer.get_images ();
    materials = importer.get_materials ();
    static_meshes = importer.get_static_meshes ();
}

void scene_graphics::create_graphics_for_images (const std::vector<std::string>& file_paths)
{
    asset_io importer (file_paths);

    std::vector<vk_texture> solo_images = importer.get_images ();
    images.insert (images.end (), solo_images.begin (), solo_images.end ());
}

void scene_graphics::bake_data_buffers ()
{

}