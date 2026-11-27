#pragma once
// windows.h here pull nothing that this file use, keep this include otherwise
// SetupAPI entry in cpp side change to a C signature (__stdcall, 8 byte stack)
// and whole program crash on device enumeration
#include <windows.h>
#include <guiddef.h>
#include <string>
#include <vector>
#include "resource/ReturnValue.h"

namespace WindowsAPI_SRV
{
    struct GetAdaptersAddressesResult {
        std::wstring friendlyName;
        std::wstring modelName;
        // GetAdaptersAddresses AdapterName, "{GUID}" form. Equal to device NetCfgInstanceId
        std::string windows_LUID;
        bool IsLoopback();
        bool IsTap();
        bool Available();
        bool Connect();
        void Disconnect();
    private:
        bool connected = false;
    };

    // Device setup class "Net", documented in
    // https://learn.microsoft.com/en-us/windows-hardware/drivers/install/system-defined-device-setup-classes-available-to-vendors
    // It is not localized on any Windows, the GUID is the same on Windows XP and Windows 11
    extern const GUID netDeviceClassGuid;

    // Return device instance path such as "ROOT\NET\0002" (same as tapinstall hwid),
    // the adapter is found by device NetCfgInstanceId equal to its AdapterName.
    // Return false if no such device present
    ReturnValue<std::wstring> GetDeviceInstancePathByNetCfgInstanceId(std::string windows_LUID);

    // Compare ignoring case, accept "{GUID}" and GUID form
    bool IsSameNetCfgInstanceId(const std::wstring& left, const std::wstring& right);

    void GetAdaptersAddresses(std::vector<GetAdaptersAddressesResult>* result);
    void GetAdaptersInfo(std::string windows_LUID, std::vector<std::string>* result);
};
