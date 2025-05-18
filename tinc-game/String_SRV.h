#pragma once
#include <string>

namespace String_SRV
{
    /// <summary>
    /// Convert std::string in local ANSI format to std::wstring
    /// Characters not encoded in local ANSI will lose
    /// </summary>
    /// <param name="input"></param>
    /// <returns></returns>
    std::wstring ForceToWstring(std::string input);
    bool IsCp437(std::wstring input);

    const std::wstring doubleQuotes = L"\"";
    const std::wstring newLine = L"\n";
    const std::wstring space = L" ";
};
