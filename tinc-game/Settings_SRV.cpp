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

void Settings_SRV::WriteLanguage(int selectedIndex)
{
	if (selectedIndex == 0) {
		Settings_SRV::config->Write(SettingKeys::language, static_cast<int>(wxLANGUAGE_UNKNOWN));
		config->Flush();
	}
	else if (selectedIndex == 1) {
		config->Write(SettingKeys::language, static_cast<int>(wxLANGUAGE_ENGLISH_US));
		config->Flush();
	}
	else if (selectedIndex == 2) {
		config->Write(SettingKeys::language, static_cast<int>(wxLANGUAGE_CHINESE_SIMPLIFIED));
		config->Flush();
	}
}

wxLanguage Settings_SRV::ReadLanguage()
{
	wxLanguage language = wxLANGUAGE_ENGLISH_US;
	int value;
	config->Read(SettingKeys::language, &value);
	if (value == wxLANGUAGE_UNKNOWN) {
		auto systemLanguage = wxLocale::GetSystemLanguage();
		if (systemLanguage == wxLANGUAGE_ENGLISH_US) {
			language = wxLANGUAGE_ENGLISH_US;
		}
		if (systemLanguage == wxLANGUAGE_CHINESE_SIMPLIFIED) {
			language = wxLANGUAGE_CHINESE_SIMPLIFIED;
		}
	}
	else {
		language = static_cast<wxLanguage>(value);
	}
	return language;
}

bool Settings_SRV::CheckIniExists()
{
	wxString ini_filename = Settings_SRV::GetIniFilePath();
	if (!wxFileName::FileExists(ini_filename)) {
		return false;
	}
	else {
		return true;
	}
}

void Settings_SRV::CreateDefaultIni()
{
	bool checkIni = Settings_SRV::CheckIniExists();
	if (!checkIni) {
		int noLanguage = wxLANGUAGE_UNKNOWN;
		config->Write(SettingKeys::language, noLanguage);
		config->Flush();
		config->Write(SettingKeys::configVersion, 0);
		config->Flush();
	}
}

wxFileConfig* Settings_SRV::config = nullptr;

const wxString SettingKeys::settings = wxT("Settings/");
const wxString SettingKeys::language = settings + wxT("Language");

const wxString SettingKeys::metadata = wxT("Metadata/");
const wxString SettingKeys::configVersion = metadata + wxT("ConfigVersion");
