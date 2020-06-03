#pragma once


#define CHECK_ERR(Func, Err) Result = Func; if (Result != ERR::SUCCESS) return Result;


enum class ERR
{
    SUCCESS
};