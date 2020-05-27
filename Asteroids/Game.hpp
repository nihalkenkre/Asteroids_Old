#pragma once

#include "Err.hpp"

class Game
{
public:
    Game () {}
    ~Game () {}

    ERR Start ();
    void Stop ();
};