#pragma once
#include <string>
#include "ReturnValue.h"
#include <vector>

namespace Networks_SRV
{
    struct GetNetworksResult {
        std::wstring name;
        std::wstring GetFullPath();
    };

    ReturnValue<std::vector<GetNetworksResult>> GetNetworks();
};
