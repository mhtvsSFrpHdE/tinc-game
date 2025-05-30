#pragma once
#include "wx/wx.h"
#include <vector>
#include "WindowsAPI_SRV.h"
#include <string>
#include "ReturnValue.h"

namespace TapDevice_SRV
{
    struct GetDefaultTapResult {
        enum class Enum {
            ConfiguredTapNotExist,
            NoTapFound,
            Unknown
        };
        WindowsAPI_SRV::GetAdaptersAddressesResult adapter;
        Enum message = Enum::Unknown;
    };

    ReturnValue<std::vector<WindowsAPI_SRV::GetAdaptersAddressesResult>> API_SRV_GetNetworkAdapterList();
    bool API_SRV_OpenNetworkControlPanel();
    const wxString openNetworkControlPanelFailedMessage = _("Failed to open \"control.exe\"");
    bool API_SRV_OpenDeviceManager();
    const wxString openDeviceManagerFailedMessage = _("Failed to open \"devmgmt.msc\"");

    extern std::wstring defaultTapFriendlyName;
    extern std::wstring defaultTapNotSet;

    extern std::vector<WindowsAPI_SRV::GetAdaptersAddressesResult> tapList;

    void Init();
    void ReloadAdapterList();

    ReturnValue<GetDefaultTapResult> GetDefaultTap();
    ReturnValue<GetDefaultTapResult> GetDefaultTap(bool refresh);
    void SetDefaultTap(WindowsAPI_SRV::GetAdaptersAddressesResult adapter);
    void UnsetDefaultTap();

    bool HasAnyInstalledTap();
};
