#pragma once

#include "static_mesh.hpp"

class actor
{
public:
    actor (const static_mesh* asset_);

    const static_mesh* asset;
};
