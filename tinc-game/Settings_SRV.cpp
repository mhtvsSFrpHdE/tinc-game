#include "Settings_SRV.h"
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/fileconf.h>
#include <wx/config.h>

wxString Settings_SRV::GetIniFilePath()
{
	// Config file
	auto a = wxStandardPaths::Get().GetExecutablePath();
	wxFileName fn(a);
	auto fnPath = fn.GetPath();

	wxString ini_filename = fnPath + wxFileName::GetPathSeparator() + "tinc-game.ini";
	return ini_filename;
}

int Settings_SRV::ChangeLanguage(int selectedIndex)
{
	wxString ini_filename = Settings_SRV::GetIniFilePath();
	wxFileConfig* config = new wxFileConfig(wxEmptyString, wxEmptyString, ini_filename);
	if (selectedIndex == 0) {
		config->Write("Settings/language", selectedIndex);
	}
	else if (selectedIndex == 1) {
		config->Write("Settings/language", selectedIndex);
	}
	delete config;
	return 0;
}

wxLanguage Settings_SRV::ReadLanguage()
{
	wxLanguage language;
	wxString ini_filename = Settings_SRV::GetIniFilePath();
	wxFileConfig* config = new wxFileConfig(wxEmptyString, wxEmptyString, ini_filename);
	int value;
	config->Read("Settings/language", &value);
	if (value == 0) {
		language = wxLANGUAGE_ENGLISH;
	}
	else if (value == 1) {
		language = wxLANGUAGE_CHINESE_SIMPLIFIED;
	}
	else {
		language = wxLANGUAGE_ENGLISH;
	}
	delete config;
	return language;
}

bool Settings_SRV::checkIni()
{
	wxString ini_filename = Settings_SRV::GetIniFilePath();
	if (!wxFileName::FileExists(ini_filename)) {
		return false;
	}
	else {
		return true;
	}
}

void Settings_SRV::createIni()
{
	bool checkIni = Settings_SRV::checkIni();
	if (!checkIni) {
		wxString ini_filename = Settings_SRV::GetIniFilePath();
		wxFileConfig* config = new wxFileConfig(wxEmptyString, wxEmptyString, ini_filename);
		config->Write(SettingKeys::language, 0);
		config->Write(SettingKeys::configVersion, 0);
		delete config;
	}
}

const wxString SettingKeys::settings = wxT("Settings/");
const wxString SettingKeys::language = settings + wxT("Language");

const wxString SettingKeys::metadata = wxT("Metadata/");
const wxString SettingKeys::configVersion = metadata + wxT("ConfigVersion");
