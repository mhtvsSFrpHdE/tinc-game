#pragma once
#include <string>
#include "Resource_SRV_TincBin.h"

namespace Resource_SRV
{
    namespace Bat {
        const std::wstring cmdRumCommand = L"/c";
        const std::wstring installTap = L"installTap.bat " + TincBin::tapInstallerDir.ToStdWstring();
        const std::wstring getTapHwid = L"getTapHwid.bat " + TincBin::tapInstallerDir.ToStdWstring();
        const std::wstring uninstallTap = L"uninstallTap.bat " + TincBin::tapInstallerDir.ToStdWstring();
        const std::wstring netsh = L"netsh.exe";
        const std::wstring netsh437 = L"netsh437.bat";
        const std::wstring netsh437Start = L"netsh437_start.bat";
        const std::wstring netsh437Result = L"netsh437_result.txt";
        const std::wstring netshArgV4 = L"ipv4";
        const std::wstring netshArgV6 = L"ipv6";
        const std::wstring ping437 = L"ping437.bat";
        const std::wstring ping437Start = L"ping437_start.bat";
        const std::wstring ping437Result = L"ping437_result.txt";
        const std::wstring tincShell = L"tincShell.bat";
        const std::wstring controlPanel = L"control.exe ncpa.cpl";
        const std::wstring deviceManager = L"start devmgmt.msc";
    }
}
