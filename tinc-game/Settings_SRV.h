#pragma once
#include <wx/wx.h>

class Settings_SRV
{
public:
	static wxString GetIniFilePath();
	static void WriteLanguage(int selectedIndex);
	static wxLanguage ReadLanguage();
	static bool CheckIniExists();
	static void CreateDefaultIni();
};

struct SettingKeys {
	static const wxString settings;
	static const wxString language;

	static const wxString metadata;
	static const wxString configVersion;
};
