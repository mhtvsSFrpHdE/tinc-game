#include "pch.h"
#include "String_SRV.h"

std::wstring String_SRV::ForceToStdWstring(std::string input)
{
    return std::wstring(input.begin(), input.end());
}

std::string String_SRV::ForceToStdString(std::wstring input)
{
    return std::string(input.begin(), input.end());
}
