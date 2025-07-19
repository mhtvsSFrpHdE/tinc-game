#pragma once
#include <wx/wx.h>
#include <wx/fileconf.h>
#include "../resource/ReturnValue.h"

enum class GetIniFilePathBy {
    Updater,
    Program,
    Networks
};

namespace Settings_SRV
{
    wxString GetIniFilePath(GetIniFilePathBy by = GetIniFilePathBy::Updater);
    wxFileName GetIniFilePathAsWxFile(GetIniFilePathBy by = GetIniFilePathBy::Updater);
    bool CheckIniExists(GetIniFilePathBy by = GetIniFilePathBy::Updater);
    void LoadConfigFile();

    const wxString arrayDelimiter1 = wxT(",");
    extern std::shared_ptr<wxFileConfig> updaterConfig;
};

namespace SettingKeys_Updater {
    const wxString files = wxT("Files/");
    const wxString files_old = files + wxT("Old");

    const wxString metadata = wxT("Metadata/");
    const wxString metadata_installedVersion = metadata + wxT("InstalledVersion");
};

namespace SettingDefaultValue_Updater {
    const long installedVersion = 0;
}
