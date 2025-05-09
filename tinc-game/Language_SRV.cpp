#include "Language_SRV.h"

std::unordered_map<Language_SRV::KnownLanguage, wxLanguage> Language_SRV::languageMap = {
	{Language_Unknown, wxLANGUAGE_UNKNOWN},
	{Language_EnglishUnitedStates, wxLANGUAGE_ENGLISH_US},
	{Language_ChineseSimplified, wxLANGUAGE_CHINESE_SIMPLIFIED}
};
