#pragma once
#include <wx/wx.h>
#include <wx/fileconf.h>
#include "ReturnValue.h"
#include "Language_SRV.h"

enum class GetIniFilePathBy {
    Program,
    Networks
};

class Settings_SRV
{
public:
    static wxString GetIniFilePath(GetIniFilePathBy by = GetIniFilePathBy::Program);
    static wxFileName GetIniFilePathAsWxFile(GetIniFilePathBy by = GetIniFilePathBy::Program);
    static void WriteLanguage(Language_SRV::KnownLanguage language);
    static wxLanguage ReadLanguage();
    static bool CheckIniExists(GetIniFilePathBy by = GetIniFilePathBy::Program);
    static void LoadConfigFile();

    static const wxString arrayDelimiter1;
    static const wxString arrayDelimiter2;
    static const wxString emptyPlaceholder1;
    static ReturnValue<wxArrayString> ReadArray(wxString settingKey, wxString delimiter = arrayDelimiter1);

    static wxFileConfig* programConfig;
    static wxFileConfig* networksConfig;
};

namespace SettingKeys_Program {
    const wxString settings = wxT("Settings/");
    const wxString settings_language = settings + wxT("Language");

    const wxString lists = wxT("Lists/");
    const wxString lists_mtuTestIp = lists + wxT("MtuTestIp");

    const wxString metadata = wxT("Metadata/");
    const wxString metadata_configVersion = metadata + wxT("ConfigVersion");
};

namespace SettingKeys_Networks {
    const wxString metadata = wxT("Metadata/");
    const wxString metadata_configVersion = metadata + wxT("ConfigVersion");

    const wxString default_rootKeyName = wxT("Default/");
    const wxString default_recentUsedNetwork = default_rootKeyName + wxT("RecentUsedNetwork");

    wxString network(wxString networkName);
    wxString network_tap(wxString networkName);
};
