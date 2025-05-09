#pragma once
#include <unordered_map>
#include <wx/wx.h>

namespace Language_SRV
{
	enum KnownLanguage {
		Language_Unknown = 0,
		Language_EnglishUnitedStates = 1,
		Language_ChineseSimplified = 2
	};

	extern std::unordered_map<Language_SRV::KnownLanguage, wxLanguage> languageMap;
};
