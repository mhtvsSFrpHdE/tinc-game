#pragma once
#include <wx/wx.h>
#include <wx/config.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/fileconf.h>
#include "ReturnValue.h"
#include "Language_SRV.h"

class Settings_SRV
{
public:
	static wxString GetIniFilePath();
	static void WriteLanguage(int selectedIndex);
	static wxLanguage ReadLanguage();
	static bool CheckIniExists();
	static void LoadConfigFile();

	static const wxString arrayDelimiter1;
	static const wxString arrayDelimiter2;
	static ReturnValue<wxArrayString> ReadArray(wxString settingKey, wxString delimiter = arrayDelimiter1);

	static wxFileConfig* config;
};

namespace SettingKeys {
	const wxString settings = wxT("Settings/");
	const wxString language = settings + wxT("Language");

	const wxString lists = wxT("Lists/");
	const wxString mtuTestIp = lists + wxT("MtuTestIp");

	const wxString metadata = wxT("Metadata/");
	const wxString configVersion = metadata + wxT("ConfigVersion");
};
