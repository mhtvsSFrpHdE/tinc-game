#pragma once
#include <string>
#include "ReturnValue.h"
#include <vector>

namespace String_SRV
{
    /// <summary>
    /// Convert std::string in local ANSI format to std::wstring
    /// Characters not encoded in local ANSI will lose
    /// </summary>
    /// <param name="input"></param>
    /// <returns></returns>
    std::wstring ForceToWstring(std::string input);

    const std::wstring doubleQuotes = L"\"";
    const std::wstring newLine = L"\n";
    const std::wstring space = L" ";
    const std::wstring ellipses = L"...";

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
