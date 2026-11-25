#include "pch.h"
#include "String_SRV.h"
#include <boost/locale/encoding.hpp>
#include <boost/locale/encoding_errors.hpp>

std::wstring String_SRV::ForceToStdWstring(std::string input)
{
    return std::wstring(input.begin(), input.end());
}

std::string String_SRV::ForceToStdString(std::wstring input)
{
    return std::string(input.begin(), input.end());
}

std::wstring String_SRV::wstringFromUtf8(const std::string& utf8String)
{
    namespace cv = boost::locale::conv;
    auto result = cv::utf_to_utf<wchar_t, char>(utf8String);
    return result;
}

std::string String_SRV::wstringToUtf8(const std::wstring& utf16String)
{
    namespace cv = boost::locale::conv;
    auto result = cv::utf_to_utf<char, wchar_t>(utf16String);
    return result;
}
