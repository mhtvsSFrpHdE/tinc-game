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

std::wstring String_SRV::utf8ToUtf16(GetConverter_Utf8AndUtf16& utfStringConverter, std::string input)
{
    auto result = utfStringConverter.from_bytes(input);
    return result;
}

std::string String_SRV::utf16ToUtf8(GetConverter_Utf8AndUtf16& utfStringConverter, std::wstring input)
{
    auto result = utfStringConverter.to_bytes(input);
    return result;
}
