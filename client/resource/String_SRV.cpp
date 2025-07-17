#include "pch.h"
#include "String_SRV.h"

std::wstring String_SRV::ForceToWstring(std::string input)
{
    return std::wstring(input.begin(), input.end());
}
