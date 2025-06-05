#pragma once
#include <string>
#include <memory>
#include <wx/filename.h>
#include <wx/dir.h>
#include "Networks_SRV.h"

/// <summary>
/// If something is reused many times, make it Resource_SRV to
/// - Avoid copy paste / type error
/// - Cache calculate result for next use
/// 
/// One time string may leave as is
/// 
/// Put important resource here allows hash them together to ensure more integrity in future
/// </summary>
namespace Resource_SRV
{
    std::wstring IntegrityCheck();

    namespace Program {
        wxFileName GetIniDir();
        const wxString programIni = wxT("tinc-game.ini");
        const wxString networksIni = wxT("tinc-game-networks.ini");
    }

    namespace TincBin {
        const wxString binDir = wxT("bin");
        wxFileName GetTincBinDir();
        const wxString tincBinDir = wxT("tinc");
        const wxString tincBin = wxT("tinc.exe");
        wxString GetTincBinAsWxStr();
        const wxString tincdBin = wxT("tincd.exe");
        wxString GetTincdBinAsWxStr();
        const wxString tincdPid = wxT("pid");
        wxFileName GetTincdPid(Networks_SRV::GetNetworksResult& network);
        const wxString tapInstallerDir = wxT("tap-win64");
        wxFileName GetTapInstallerDir();
        const wxString tapInstallerBin = wxT("tapinstall.exe");
        const wxString tapInstallerInf = wxT("OemWin2k.inf");
        const wxString tapInstallerCat = wxT("tap0901.cat");
        const wxString tapInstallerSys = wxT("tap0901.sys");
    }

    namespace Bat {
        const std::wstring cmdRumCommand = L"/c";
        const std::wstring installTap = L"installTap.bat";
        const std::wstring getTapHwid = L"getTapHwid.bat";
        const std::wstring uninstallTap = L"uninstallTap.bat";
        const std::wstring netsh437 = L"netsh437.bat";
        const std::wstring netshArgV4 = L"ipv4";
        const std::wstring netshArgV6 = L"ipv6";
    }

    namespace Networks {
        const wxString networksDir = wxT("networks");
        std::unique_ptr<wxDir> GetNetworksDirAsWxDir();
        wxFileName GetNetworksDir();
    }
};
