#pragma once
#include <string>

class String_SRV
{
public:
	/// <summary>
	/// Convert std::string in local ANSI format to std::wstring
	/// Characters not encoded in local ANSI will lose
	/// </summary>
	/// <param name="input"></param>
	/// <returns></returns>
	static std::wstring ForceToWstring(std::string input);
	static bool IsCp437(std::wstring input);
};
