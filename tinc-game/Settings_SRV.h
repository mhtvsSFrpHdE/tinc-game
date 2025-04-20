#pragma once
#include <wx/wx.h>

class Settings_SRV
{
public:
	static wxString GetIniFilePath();
	static int ChangeLanguage(int selectedIndex);
	static wxLanguage ReadLanguage();
	static bool checkIni();
	static void createIni();
};

struct SettingKeys {
	static const wxString settings;
	static const wxString language;

	static const wxString metadata;
	static const wxString configVersion;
};
