#pragma once
#include <wx/wx.h>
#include <wx/fileconf.h>
#include "ReturnValue.h"
#include "Language_SRV.h"

enum class GetIniFilePathBy {
    Program,
    Networks
};

namespace Settings_SRV
{
    wxString GetIniFilePath(GetIniFilePathBy by = GetIniFilePathBy::Program);
    wxFileName GetIniFilePathAsWxFile(GetIniFilePathBy by = GetIniFilePathBy::Program);
    void WriteLanguage(Language_SRV::KnownLanguage language);
    wxLanguage ReadLanguage();
    bool CheckIniExists(GetIniFilePathBy by = GetIniFilePathBy::Program);
    void LoadConfigFile();

    const wxString arrayDelimiter1 = wxT(",");
    const wxString arrayDelimiter2 = wxT("|");
    const wxString emptyPlaceholder1 = wxT("|");
    ReturnValue<wxArrayString> ReadArray(wxString settingKey, wxString delimiter = arrayDelimiter1);

    extern wxFileConfig* programConfig;
    extern wxFileConfig* networksConfig;
};

namespace SettingKeys_Program {
    const wxString settings = wxT("Settings/");
    const wxString settings_language = settings + wxT("Language");
    const wxString settings_gameMode = settings + wxT("GameApp.exe");

    const wxString lists = wxT("Lists/");
    const wxString lists_mtuTestIp = lists + wxT("MtuTestIp");
    const wxString lists_gameModeGames = lists + wxT("GameModeGames");

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
    wxString network_verbose(wxString networkName);
    wxString network_autoStart(wxString networkName);
    wxString network_gameMode(wxString networkName);
    wxString network_port(wxString networkName);
    wxString network_setMetric(wxString networkName);
};
