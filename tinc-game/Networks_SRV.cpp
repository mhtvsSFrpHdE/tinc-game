#include "Networks_SRV.h"
#include <wx/filename.h>
#include <wx/dir.h>

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
        subFolder.name = filename.ToStdWstring();
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
    file.AppendDir(name);
    file.Normalize();

    auto fileFullPath = file.GetFullPath().ToStdWstring();
    auto stripEndBackslash = fileFullPath.substr(0, fileFullPath.length() - 1);
    return stripEndBackslash;
}
