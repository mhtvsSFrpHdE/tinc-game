#include "Language_SRV.h"

std::vector<Language_SRV::KnownLanguage> Language_SRV::languageList = {
    Language_SRV::KnownLanguage::Unknown,
    Language_SRV::KnownLanguage::EnglishUnitedStates,
    Language_SRV::KnownLanguage::ChineseSimplified
};

std::unordered_map<Language_SRV::KnownLanguage, int> Language_SRV::languageKeyMap;

std::unordered_map<Language_SRV::KnownLanguage, wxString> Language_SRV::languageNameMap;

std::unordered_map<Language_SRV::KnownLanguage, wxLanguage> Language_SRV::wxLanguageMap = {
    { Language_SRV::KnownLanguage::Unknown, wxLANGUAGE_UNKNOWN },
    { Language_SRV::KnownLanguage::EnglishUnitedStates, wxLANGUAGE_ENGLISH_US },
    { Language_SRV::KnownLanguage::ChineseSimplified, wxLANGUAGE_CHINESE_SIMPLIFIED }
};


void Language_SRV::Init()
{
    languageNameMap.insert({ Language_SRV::KnownLanguage::Unknown, _("Auto (System Default)") });
    languageNameMap.insert({ Language_SRV::KnownLanguage::EnglishUnitedStates, _("English (United States)") });
    languageNameMap.insert({ Language_SRV::KnownLanguage::ChineseSimplified, _("Chinese Simplified") });

    // Use for loop make sure languageKeyMap order matchs languageList
    // So you don't have to manually track order twice
    for (size_t i = 0; i < languageList.size(); i++)
    {
        languageKeyMap.insert({ languageList[i], i });
    }
}
