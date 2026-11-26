#pragma once
#include <wx/hashmap.h>

/// <summary>
/// Patch for standard library difference between compilers.
///
/// MinGW 4.8.2 (libstdc++) only forward declares std::hash, there is no
/// std::hash<wxString>. MSVC's unordered_map happens to accept it anyway,
/// so this never surfaced until the compiler changed.
///
/// Include this where unordered_map / unordered_set is keyed by wxString.
/// Equality keeps wxString operator==, which is what the MSVC build did.
/// 
/// MSVC has a weird behavior, when use "if defined(__GNUC__)"
/// to hide these code, it won't compile and error message like
/// MSVC\14.44.35207\include\xhash(118): error C2064
/// </summary>
namespace std
{
    template<> struct hash<wxString>
    {
        size_t operator()(const wxString& key) const;
    };
}
