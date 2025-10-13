#pragma once
#include <wx/filename.h>

namespace Resource_SRV
{
    namespace TincBin {
        const wxString tincBinDir = wxT("tinc");
        const wxString tincBin = wxT("tinc.exe");
        wxFileName GetTincBinDir();
        wxString GetTincBinAsWxStr();
        const wxString tincdBin = wxT("tincd.exe");
        wxString GetTincdBinAsWxStr();
        wxFileName GetTincdBin();
        wxFileName GetTincdGameModeBin(wxString gameModeValue);
        const wxString tincdPid = wxT("pid");
        wxFileName GetTincdPid(std::wstring networkFullPath);
        const wxString tapInstallerDir = wxT("tap-win64");
        wxFileName GetTapInstallerDir();
        const wxString tapInstallerBin = wxT("tapinstall.exe");
        const wxString tapInstallerInf = wxT("OemWin2k.inf");
        const wxString tapInstallerCat = wxT("tap0901.cat");
        const wxString tapInstallerSys = wxT("tap0901.sys");
    }
}
