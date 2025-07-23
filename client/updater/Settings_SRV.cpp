#include "Settings_SRV.h"
#include "..\resource\Resource_SRV.h"

wxString Settings_SRV::GetIniFilePath(GetIniFilePathBy by)
{
    auto iniFile = GetIniFilePathAsWxFile(by);
    return iniFile.GetFullPath();
}

wxFileName Settings_SRV::GetIniFilePathAsWxFile(GetIniFilePathBy by)
{
    namespace rsp = Resource_SRV::Program;

    auto iniFile = rsp::GetIniDir();

    if (by == GetIniFilePathBy::Updater) {
        iniFile.SetName(rsp::updaterIni);
        return iniFile;
    }
    if (by == GetIniFilePathBy::Program) {
        iniFile.SetName(rsp::programIni);
        return iniFile;
    }
    if (by == GetIniFilePathBy::Networks) {
        iniFile.SetName(rsp::networksIni);
        return iniFile;
    }

    return iniFile;
}

bool Settings_SRV::CheckIniExists(GetIniFilePathBy by)
{
    auto iniFile = Settings_SRV::GetIniFilePathAsWxFile(by);
    auto exists = iniFile.Exists();
    return exists;
}

void Settings_SRV::LoadConfigFile()
{
    {
        auto iniType = GetIniFilePathBy::Updater;
        namespace sk = SettingKeys_Updater;

        bool checkIni = Settings_SRV::CheckIniExists(iniType);
        updaterConfig = std::shared_ptr<wxFileConfig>(new wxFileConfig(wxEmptyString, wxEmptyString, Settings_SRV::GetIniFilePath(iniType)));
        if (!checkIni) {
            updaterConfig->Write(sk::metadata_installedVersion, 0);
            updaterConfig->Flush();
        }
    }
}

ReturnValue<wxArrayString> Settings_SRV::ReadArray(wxFileConfig* config, wxString settingKey, wxString delimiter)
{
    auto result = ReturnValue<wxArrayString>();

    wxString readConfig;
    bool readConfigSuccess = config->Read(settingKey, &readConfig);
    if (readConfigSuccess == false) {
        return result;
    }

    wxArrayString split = wxSplit(readConfig, Settings_SRV::arrayDelimiter1[0]);
    result.returnBody = split;
    result.success = true;

    return result;
}

void Settings_SRV::WriteArray(wxString settingKey, wxString delimiter)
{

}

std::shared_ptr<wxFileConfig> Settings_SRV::updaterConfig;
