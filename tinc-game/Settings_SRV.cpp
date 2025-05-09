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
	namespace sk = SettingKeys;

	if (selectedIndex == 0) {
		Settings_SRV::config->Write(sk::language, static_cast<int>(wxLANGUAGE_UNKNOWN));
		config->Flush();
	}
	else if (selectedIndex == 1) {
		config->Write(sk::language, static_cast<int>(wxLANGUAGE_ENGLISH_US));
		config->Flush();
	}
	else if (selectedIndex == 2) {
		config->Write(sk::language, static_cast<int>(wxLANGUAGE_CHINESE_SIMPLIFIED));
		config->Flush();
	}
}

wxLanguage Settings_SRV::ReadLanguage()
{
	namespace sk = SettingKeys;

	wxLanguage language = wxLANGUAGE_ENGLISH_US;
	int value;
	config->Read(sk::language, &value);
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

void Settings_SRV::LoadConfigFile()
{
	namespace sk = SettingKeys;

	bool checkIni = Settings_SRV::CheckIniExists();
	config = new wxFileConfig(wxEmptyString, wxEmptyString, Settings_SRV::GetIniFilePath());
	if (!checkIni) {
		config->Write(sk::language, static_cast<int>(wxLANGUAGE_UNKNOWN));
		config->Write(sk::mtuTestIp, wxT("1.1.1.1, 8.8.8.8, 10.255.60.1"));
		config->Write(sk::configVersion, 0);
		config->Flush();
	}
}

ReturnValue<wxArrayString> Settings_SRV::ReadArray(wxString delimiter, wxString settingKey)
{
	auto result = ReturnValue<wxArrayString>();

	wxString readConfig;
	bool readConfigSuccess = config->Read(SettingKeys::mtuTestIp, &readConfig);
	if (readConfigSuccess == false) {
		return result;
	}

	wxArrayString split = wxSplit(readConfig, Settings_SRV::arrayDelimiter1[0]);
	result.returnBody = split;
	result.success = true;

	return result;
}

wxFileConfig* Settings_SRV::config = nullptr;
const wxString Settings_SRV::arrayDelimiter1 = wxT(",");
const wxString Settings_SRV::arrayDelimiter2 = wxT("|");
