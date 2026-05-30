#pragma once
#include <string>
#include "ReturnValue.h"
#include <vector>
#include <codecvt>

namespace String_SRV
{
    /// <summary>
    /// Convert std::string in local ANSI format to std::wstring
    /// Characters not encoded in local ANSI will lose
    /// </summary>
    /// <param name="input"></param>
    /// <returns></returns>
    std::wstring ForceToStdWstring(std::string input);

    /// <summary>
    /// Convert std::wstring to local ANSI format std::string
    /// Characters not encoded in local ANSI will lose
    /// </summary>
    /// <param name="input"></param>
    /// <returns></returns>
    std::string ForceToStdString(std::wstring input);

    /// <summary>
    /// !!! This thing is not safe in multi thread
    /// Don't share instance between thread
    /// 
    /// Mainly for CPR HTTP requests and boost JSON
    /// 
    /// Says Boost.JSON does not natively support std::wstring or wchar_t
    /// because the standard JSON specification (RFC 8259)
    /// is strictly defined as a sequence of bytes, typically encoded in UTF-8
    /// 
    /// Also says CPR and libcurl internally use UTF-8 encoded std::string
    /// 
    /// As for Windows, std::wstring refer to UTF-16
    /// 
    /// Usage
    /// 1. `GetConverter_Utf8ToUtf16 urfStringConverter;`, create on stack, don't new
    /// 2. std::wstring to std::string, call `utf8ToUtf16(urfStringConverter, std::wstring)`
    /// 2. std::string to std::wstring, call `utf16ToUtf8(urfStringConverter, std::string)`
    /// </summary>
    typedef std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> GetConverter_Utf8AndUtf16;
    std::wstring utf8ToUtf16(GetConverter_Utf8AndUtf16& utfStringConverter, std::string input);
    std::string utf16ToUtf8(GetConverter_Utf8AndUtf16& utfStringConverter, std::wstring input);

    const std::wstring doubleQuotes = L"\"";
    const std::wstring newLine = L"\n";
    const std::wstring space = L" ";
    const std::wstring ellipses = L"...";
    const std::wstring emptyString = L"";

    /// <summary>
    /// This template function join vector objects to single string, like `std::join
    /// Generate std::wstring or std::string depends on "ReturnType"
    /// Use toStringFunctionPointer to know how to convert object to string
    /// 
    /// Example:
    ///     std::wstring wxFileNameToString(wxFileName&amp; file) { return file.GetName().ToStdWstring(); }
    ///     JoinVectorNonStringObject&lt;std::wstring, wxFileName&gt;(oldFiles, delimiter, &amp;wxFileNameToString);
    /// </summary>
    /// <typeparam name="ReturnType"></typeparam>
    /// <typeparam name="ObjectType"></typeparam>
    /// <param name="container"></param>
    /// <param name="delimiter"></param>
    /// <param name="toStringFunctionPointer"></param>
    /// <returns></returns>
    template<typename ObjectType, typename ReturnType = std::wstring>
    ReturnValue<ReturnType> JoinVectorNonStringObject(std::vector<ObjectType>& container, ReturnType delimiter, ReturnType(*toStringFunctionPointer) (ObjectType&))
    {
        ReturnValue<ReturnType> result;

        if (container.size() == 0) {
            return result;
        }

        // If ReturnType is std::wstring, StringStreamType will be std::wstringstream
        // Else StringStreamType will be std::stringstream
        using StringStreamType = typename std::conditional<std::is_same<std::wstring, ReturnType>::value, std::wstringstream, std::stringstream>::type;
        StringStreamType joinedString;

        if (container.size() > 0) {
            joinedString << toStringFunctionPointer(container[0]);
        }
        if (container.size() > 1) {
            for (size_t i = 1; i < container.size(); i++)
            {
                joinedString << delimiter << toStringFunctionPointer(container[i]);
            }
        }

        result.success = true;
        result.returnBody = joinedString.str();
        return result;
    }
};
