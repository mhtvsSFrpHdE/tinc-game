#include "Language_SRV.h"

std::unordered_map<Language_SRV::KnownLanguage, wxLanguage> Language_SRV::languageMap = {
	{Language_Unknown, wxLANGUAGE_UNKNOWN},
	{Language_EnglishUnitedStates, wxLANGUAGE_ENGLISH_US},
	{Language_ChineseSimplified, wxLANGUAGE_CHINESE_SIMPLIFIED}
};

std::unordered_map<Language_SRV::KnownLanguage, wxString> Language_SRV::languageNameMap;

std::vector<Language_SRV::KnownLanguage> Language_SRV::languageList = {
	Language_Unknown,
	Language_EnglishUnitedStates,
	Language_ChineseSimplified
};

void Language_SRV::Init()
{
	languageNameMap.insert({ Language_Unknown, _("Auto (System Default)") });
	languageNameMap.insert({ Language_EnglishUnitedStates, _("English (United States)") });
	languageNameMap.insert({ Language_ChineseSimplified, _("Chinese Simplified") });
}
