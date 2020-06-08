#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <memory>

#include "err.hpp"


class scene_graphics;


class scene
{
public:
    scene ();
    ~scene ();

    virtual void init ();
    virtual void process_keyboard_input (WPARAM WParam, LPARAM LParam);
    virtual void main_loop ();

protected:
    void import_image (std::string Name);
    void import_mesh (std::string Name);
    
    void commit_assets ();

private:
    std::vector <std::string> image_names;
    std::vector <std::string> mesh_names;

    std::unique_ptr<scene_graphics> graphics = std::make_unique<scene_graphics> ();
};
