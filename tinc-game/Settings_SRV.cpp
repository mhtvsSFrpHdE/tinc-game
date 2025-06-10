#include "Settings_SRV.h"
#include "Resource_SRV.h"

wxString Settings_SRV::GetIniFilePath(GetIniFilePathBy by)
{
    auto iniFile = GetIniFilePathAsWxFile(by);
    return iniFile.GetFullPath();
}

wxFileName Settings_SRV::GetIniFilePathAsWxFile(GetIniFilePathBy by)
{
    namespace rsp = Resource_SRV::Program;

    auto iniFile = rsp::GetIniDir();

    if (by == GetIniFilePathBy::Program) {
        iniFile.SetName(rsp::programIni);
        return iniFile;
    }
    if (by == GetIniFilePathBy::Networks) {
        iniFile.SetName(rsp::networksIni);
        return iniFile;
    }
}

void Settings_SRV::WriteLanguage(Language_SRV::KnownLanguage language)
{
    programConfig->Write(SettingKeys_Program::settings_language, static_cast<int>(language));
}

wxLanguage Settings_SRV::ReadLanguage()
{
    namespace sk = SettingKeys_Program;
    namespace ls = Language_SRV;

    wxLanguage language = wxLANGUAGE_ENGLISH_US;
    int value;
    programConfig->Read(sk::settings_language, &value);
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
    auto iniFile = Settings_SRV::GetIniFilePathAsWxFile(by);
    auto exists = iniFile.Exists();
    return exists;
}

void Settings_SRV::LoadConfigFile()
{
    namespace ls = Language_SRV;

    {
        auto iniType = GetIniFilePathBy::Program;
        namespace sk = SettingKeys_Program;

        bool checkIni = Settings_SRV::CheckIniExists(iniType);
        programConfig = new wxFileConfig(wxEmptyString, wxEmptyString, Settings_SRV::GetIniFilePath(iniType));
        if (!checkIni) {
            programConfig->Write(sk::metadata_configVersion, 0);
            programConfig->Write(sk::settings_language, static_cast<int>(ls::KnownLanguage::Unknown));
            programConfig->Write(sk::settings_gameMode, wxT("GameApp.exe"));
            programConfig->Write(sk::lists_mtuTestIp, wxT("1.1.1.1, 8.8.8.8, 10.255.60.1"));
            programConfig->Write(sk::lists_gameModeGames, wxT("GameApp.exe, javaw.exe"));
            programConfig->Flush();
        }
    }

    {
        auto iniType = GetIniFilePathBy::Networks;
        namespace sk = SettingKeys_Networks;

        bool checkIni = Settings_SRV::CheckIniExists(iniType);
        networksConfig = new wxFileConfig(wxEmptyString, wxEmptyString, Settings_SRV::GetIniFilePath(iniType));
        if (!checkIni) {
            networksConfig->Write(sk::metadata_configVersion, 0);
            networksConfig->Write(sk::default_recentUsedNetwork, emptyPlaceholder1);
            networksConfig->Flush();
        }
    }

    ls::Init();
}

ReturnValue<wxArrayString> Settings_SRV::ReadArray(wxString settingKey, wxString delimiter)
{
    auto result = ReturnValue<wxArrayString>();

    wxString readConfig;
    bool readConfigSuccess = programConfig->Read(settingKey, &readConfig);
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

wxString SettingKeys_Networks::network(wxString networkName)
{
    const wxString network = wxT("Network_");
    const wxString pathSplitter = wxT("/");

    auto result = network + networkName + pathSplitter;
    return result;
}

wxString SettingKeys_Networks::network_tap(wxString networkName)
{
    const wxString keyName = wxT("VirtualNetworkAdapter");

    auto result = network(networkName) + keyName;
    return result;
}

wxString SettingKeys_Networks::network_verbose(wxString networkName)
{
    const wxString keyName = wxT("ShowDetailedLiveLog");

    auto result = network(networkName) + keyName;
    return result;
}

wxString SettingKeys_Networks::network_autoStart(wxString networkName)
{
    const wxString keyName = wxT("AutoConnectOnStart");

    auto result = network(networkName) + keyName;
    return result;
}

wxString SettingKeys_Networks::network_gameMode(wxString networkName)
{
    const wxString keyName = wxT("GameMode");

    auto result = network(networkName) + keyName;
    return result;
}

wxString SettingKeys_Networks::network_port(wxString networkName)
{
    const wxString keyName = wxT("Port");

    auto result = network(networkName) + keyName;
    return result;
}

wxString SettingKeys_Networks::network_setMetric(wxString networkName)
{
    const wxString keyName = wxT("SetMetric");

    auto result = network(networkName) + keyName;
    return result;
}
