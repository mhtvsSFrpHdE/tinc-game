#include "Networks_SRV.h"
#include <wx/filename.h>
#include <wx/dir.h>
#include "Settings_SRV.h"

ReturnValue<std::vector<Networks_SRV::GetNetworksResult>> Networks_SRV::GetNetworks()
{
    auto result = ReturnValue<std::vector<Networks_SRV::GetNetworksResult>>();

    wxDir dir("networks");
    if (!dir.IsOpened())
    {
        return result;
    }

    wxString filename;
    std::vector<wxString> collect;
    bool cont = dir.GetFirst(&filename, wxEmptyString, wxDIR_DIRS);
    while (cont)
    {
        GetNetworksResult subFolder;
        subFolder.networkName = filename.ToStdWstring();
        auto settingsKeyName = SettingKeys_Networks::tap(subFolder.networkName);
        subFolder.recentUsedTapName = Settings_SRV::networksConfig->Read(settingsKeyName);
        result.returnBody.push_back(subFolder);

        cont = dir.GetNext(&filename);
    }
    if (result.returnBody.size() > 0) {
        result.success = true;
    }
    return result;
}

std::wstring Networks_SRV::GetNetworksResult::GetFullPath()
{
    wxFileName file;
    file.AppendDir("networks");
    file.AppendDir(networkName);
    file.Normalize();

    auto fileFullPath = file.GetFullPath().ToStdWstring();
    auto stripEndBackslash = fileFullPath.substr(0, fileFullPath.length() - 1);
    return stripEndBackslash;
}
