#pragma once
#include <wx/wx.h>
#include <wx/config.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/fileconf.h>

class Settings_SRV
{
public:
	static wxString GetIniFilePath();
	static void WriteLanguage(int selectedIndex);
	static wxLanguage ReadLanguage();
	static bool CheckIniExists();
	static void LoadConfigFile();

	static wxFileConfig* config;
};

struct SettingKeys {
	static const wxString settings;
	static const wxString language;

	static const wxString metadata;
	static const wxString configVersion;
};