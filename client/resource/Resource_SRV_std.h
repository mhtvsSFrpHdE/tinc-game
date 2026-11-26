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
/// </summary>
namespace std
{
    template<> struct hash<wxString>
    {
        size_t operator()(const wxString& key) const;
    };
}
