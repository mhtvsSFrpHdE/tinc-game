#pragma once
#include <unordered_map>
#include <vector>
#include <wx/wx.h>
#include "Resource_SRV_std.h"

namespace Language_SRV
{
    enum class KnownLanguage {
        Unknown = 0,
        EnglishUnitedStates = 1,
        ChineseSimplified = 2
    };

    extern std::vector<Language_SRV::KnownLanguage> languageList;
    extern std::unordered_map<Language_SRV::KnownLanguage, int> languageKeyMap;
    extern std::unordered_map<Language_SRV::KnownLanguage, wxString> languageNameMap;
    extern std::unordered_map<Language_SRV::KnownLanguage, wxLanguage> wxLanguageMap;

    void Init();
};
