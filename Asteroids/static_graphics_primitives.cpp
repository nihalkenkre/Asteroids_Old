#include "static_graphics_primitives.hpp"

#include <Windows.h>

static_graphics_primitive::static_graphics_primitive ()
{
    OutputDebugString (L"static_graphics_primitive::static_graphics_primitive\n");
}

static_graphics_primitive::static_graphics_primitive (const tinygltf::Primitive& primitive, const tinygltf::Model& model)
{
    OutputDebugString (L"static_graphics_primitive::static_graphics_primitive primitive model\n");
}

static_graphics_primitive::static_graphics_primitive (const static_graphics_primitive& other)
{
    OutputDebugString (L"static_graphics_primitive::static_graphics_primitive Copy constructor\n");
}

static_graphics_primitive& static_graphics_primitive::operator=(const static_graphics_primitive& other)
{
    OutputDebugString (L"static_graphics_primitive::static_graphics_primitive Copy assignment\n");

    return *this;
}

static_graphics_primitive::static_graphics_primitive (const static_graphics_primitive&& other) noexcept
{
    OutputDebugString (L"static_graphics_primitive::static_graphics_primitive Move constructor\n");
}

static_graphics_primitive& static_graphics_primitive::operator=(const static_graphics_primitive&& other) noexcept
{
    OutputDebugString (L"static_graphics_primitive::static_graphics_primitive Move assignment\n");

    return *this;
}

static_graphics_primitive::~static_graphics_primitive ()
{
    OutputDebugString (L"static_graphics_primitive::~static_graphics_primitive\n");
}


static_graphics_primitives::static_graphics_primitives ()
{
    OutputDebugString (L"static_graphics_primitives::static_graphics_primitives\n");
}

static_graphics_primitives::static_graphics_primitives (const static_graphics_primitives& other)
{
    OutputDebugString (L"static_graphics_primitives::static_graphics_primitives Copy constructor\n");
}

static_graphics_primitives& static_graphics_primitives::operator=(const static_graphics_primitives& other)
{
    OutputDebugString (L"static_graphics_primitives::static_graphics_primitives Copy assignment\n");

    return *this;
}

static_graphics_primitives::static_graphics_primitives (const static_graphics_primitives&& other) noexcept
{
    OutputDebugString (L"static_graphics_primitives::static_graphics_primitives Move constructor\n");
}

static_graphics_primitives& static_graphics_primitives::operator=(const static_graphics_primitives&& other) noexcept
{
    OutputDebugString (L"static_graphics_primitives::static_graphics_primitives Move assignment\n");

    return *this;
}

static_graphics_primitives::~static_graphics_primitives ()
{
    OutputDebugString (L"static_graphics_primitives::~static_graphics_primitives\n");
}