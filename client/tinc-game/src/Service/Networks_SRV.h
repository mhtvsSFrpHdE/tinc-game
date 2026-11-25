#pragma once
#include "resource/ReturnValue.h"
#include <vector>
#include "WindowsAPI_SRV.h"
#include <string>

namespace Networks_SRV
{
    struct GetNetworksResult {
        std::wstring networkName;
        std::wstring recentUsedTapName;
        std::wstring GetFullPath();
    };

    ReturnValue<std::vector<GetNetworksResult>> GetNetworks();
};
