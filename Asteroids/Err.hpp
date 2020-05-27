#pragma once

#define CHECK_ERR (func, err) result = func; if (result != ERR::SUCCESS) return result;

enum class ERR
{
    SUCCESS
};