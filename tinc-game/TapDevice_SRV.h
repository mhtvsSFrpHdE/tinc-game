#pragma once
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

    extern std::wstring defaultVirtualNetworkAdapter;
    extern std::wstring emptyPlaceholder;

    extern std::vector<WindowsAPI_SRV::GetAdaptersAddressesResult> adapterList;

    void Init();
    void ReloadAdapterList();
    ReturnValue<GetDefaultTapResult> GetDefaultTap();
    void SetDefaultTap(WindowsAPI_SRV::GetAdaptersAddressesResult adapter);
};
