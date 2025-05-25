#include "Settings_SRV.h"
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/fileconf.h>
#include <wx/config.h>
#include "TapDevice_SRV.h"

wxString Settings_SRV::GetIniFilePath(GetIniFilePathBy by)
{
    auto exePath = wxStandardPaths::Get().GetExecutablePath();
    wxFileName exeFile(exePath);
    auto exeDir = exeFile.GetPath();

    if (by == GetIniFilePathBy::Program) {
        wxString ini_filename = exeDir + wxFileName::GetPathSeparator() + "tinc-game.ini";
        return ini_filename;
    }
    if (by == GetIniFilePathBy::Networks) {
        wxString ini_filename = exeDir + wxFileName::GetPathSeparator() + "tinc-game-networks.ini";
        return ini_filename;
    }
}

void Settings_SRV::WriteLanguage(Language_SRV::KnownLanguage language)
{
    Settings_SRV::programConfig->Write(SettingKeys_Program::language, static_cast<int>(language));
    programConfig->Flush();
}

wxLanguage Settings_SRV::ReadLanguage()
{
    namespace sk = SettingKeys_Program;
    namespace ls = Language_SRV;

    wxLanguage language = wxLANGUAGE_ENGLISH_US;
    int value;
    programConfig->Read(sk::language, &value);
    if (value == static_cast<int>(ls::KnownLanguage::Unknown)) {
        auto getSystemLanguage = wxLocale::GetSystemLanguage();
        language = static_cast<wxLanguage>(getSystemLanguage);
    }
    else {
        language = ls::wxLanguageMap[static_cast<ls::KnownLanguage>(value)];
    }
    return language;
}

bool Settings_SRV::CheckIniExists(GetIniFilePathBy by)
{
    wxString ini_filename = Settings_SRV::GetIniFilePath(by);
    if (!wxFileName::FileExists(ini_filename)) {
        return false;
    }
    else {
        return true;
    }
}

void Settings_SRV::LoadConfigFile()
{
    namespace ls = Language_SRV;
    namespace ts = TapDevice_SRV;

    {
        auto iniType = GetIniFilePathBy::Program;
        namespace sk = SettingKeys_Program;

        bool checkIni = Settings_SRV::CheckIniExists(iniType);
        programConfig = new wxFileConfig(wxEmptyString, wxEmptyString, Settings_SRV::GetIniFilePath(iniType));
        if (!checkIni) {
            programConfig->Write(sk::configVersion, 0);
            programConfig->Write(sk::language, static_cast<int>(ls::KnownLanguage::Unknown));
            programConfig->Write(sk::mtuTestIp, wxT("1.1.1.1, 8.8.8.8, 10.255.60.1"));
            programConfig->Flush();
        }
    }

    {
        auto iniType = GetIniFilePathBy::Networks;
        namespace sk = SettingKeys_Networks;

        bool checkIni = Settings_SRV::CheckIniExists(iniType);
        networksConfig = new wxFileConfig(wxEmptyString, wxEmptyString, Settings_SRV::GetIniFilePath(iniType));
        if (!checkIni) {
            networksConfig->Write(sk::configVersion, 0);
            networksConfig->Write(sk::defaultNetwork, emptyPlaceholder1);
            networksConfig->Write(sk::defaultTap, emptyPlaceholder1);
            networksConfig->Flush();
        }
    }

    ls::Init();
    ts::Init();
}

ReturnValue<wxArrayString> Settings_SRV::ReadArray(wxString delimiter, wxString settingKey)
{
    auto result = ReturnValue<wxArrayString>();

    wxString readConfig;
    bool readConfigSuccess = programConfig->Read(SettingKeys_Program::mtuTestIp, &readConfig);
    if (readConfigSuccess == false) {
        return result;
    }

    wxArrayString split = wxSplit(readConfig, Settings_SRV::arrayDelimiter1[0]);
    result.returnBody = split;
    result.success = true;

    return result;
}

wxFileConfig* Settings_SRV::programConfig = nullptr;
wxFileConfig* Settings_SRV::networksConfig = nullptr;

const wxString Settings_SRV::arrayDelimiter1 = wxT(",");
const wxString Settings_SRV::arrayDelimiter2 = wxT("|");
const wxString Settings_SRV::emptyPlaceholder1 = wxT("|");

wxString SettingKeys_Networks::network(wxString networkName)
{
    const wxString network = wxT("Network_");
    const wxString pathSplitter = wxT("/");

    auto result = network + networkName + pathSplitter;
    return result;
}

wxString SettingKeys_Networks::tap(wxString networkName)
{
    const wxString keyName = wxT("VirtualNetworkAdapter");

    auto result = network(networkName) + keyName;
    return result;
}
