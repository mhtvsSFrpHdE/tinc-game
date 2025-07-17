#pragma once
#include "wx/wx.h"
#include <vector>
#include "WindowsAPI_SRV.h"
#include <string>
#include "..\resource\ReturnValue.h"

namespace TapDevice_SRV
{
    ReturnValue<std::vector<WindowsAPI_SRV::GetAdaptersAddressesResult>> API_SRV_GetNetworkAdapterList();
    bool API_SRV_OpenNetworkControlPanel();
    const wxString openNetworkControlPanelFailedMessage = _("Failed to open \"control.exe\"");
    bool API_SRV_OpenDeviceManager();
    const wxString openDeviceManagerFailedMessage = _("Failed to open \"devmgmt.msc\"");

    extern std::vector<WindowsAPI_SRV::GetAdaptersAddressesResult> tapList;

    void ReloadAdapterList();

    ReturnValue<WindowsAPI_SRV::GetAdaptersAddressesResult> GetFirstTap();
    ReturnValue<WindowsAPI_SRV::GetAdaptersAddressesResult> GetFirstTap(bool refresh);

    bool HasAnyInstalledTap();
};
