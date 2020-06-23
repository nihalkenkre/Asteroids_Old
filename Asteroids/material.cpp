#include "material.hpp"
#include "image.hpp"

material::material (const tinygltf::Material& material, const tinygltf::Model& model) : name (material.name)
{
    OutputDebugString (L"material::material material\n");

    base_image = std::make_unique<image> (model.images[model.textures[material.pbrMetallicRoughness.baseColorTexture.index].source], model);
    base_color_factor = { 
        (float)material.pbrMetallicRoughness.baseColorFactor.at (0), 
        (float)material.pbrMetallicRoughness.baseColorFactor.at (1), 
        (float)material.pbrMetallicRoughness.baseColorFactor.at (2), 
        (float)material.pbrMetallicRoughness.baseColorFactor.at (3)
    };
}
