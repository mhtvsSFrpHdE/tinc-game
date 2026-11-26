#include "pch.h"
#include "Resource_SRV_std.h"

std::size_t std::hash<wxString>::operator()(const wxString& key) const
{
    return static_cast<std::size_t>(wxStringHash::stringHash(key.wx_str()));
}
