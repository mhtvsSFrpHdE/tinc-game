#include "Language_SRV.h"

std::unordered_map<Language_SRV::KnownLanguage, wxLanguage> Language_SRV::languageMap = {
    { KnownLanguage::Unknown, wxLANGUAGE_UNKNOWN },
    { KnownLanguage::EnglishUnitedStates, wxLANGUAGE_ENGLISH_US },
    { KnownLanguage::ChineseSimplified, wxLANGUAGE_CHINESE_SIMPLIFIED }
};

std::unordered_map<Language_SRV::KnownLanguage, wxString> Language_SRV::languageNameMap;

std::vector<Language_SRV::KnownLanguage> Language_SRV::languageList = {
    KnownLanguage::Unknown,
    KnownLanguage::EnglishUnitedStates,
    KnownLanguage::ChineseSimplified
};

void Language_SRV::Init()
{
    languageNameMap.insert({ KnownLanguage::Unknown, _("Auto (System Default)") });
    languageNameMap.insert({ KnownLanguage::EnglishUnitedStates, _("English (United States)") });
    languageNameMap.insert({ KnownLanguage::ChineseSimplified, _("Chinese Simplified") });
}
