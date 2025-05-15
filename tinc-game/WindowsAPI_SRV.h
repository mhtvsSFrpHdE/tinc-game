#pragma once
#include <vector>
#include <string>

namespace WindowsAPI_SRV
{
    struct GetAdaptersAddressesResult {
        std::wstring friendlyName;
        std::wstring modelName;
        std::string windows_LUID;
        bool isLoopback();
        bool isTapDevice();
    };

    void GetAdaptersAddresses(std::vector<GetAdaptersAddressesResult>* result);
};
