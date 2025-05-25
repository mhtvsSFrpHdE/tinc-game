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
        bool isTap();
        bool isConnected();
        bool Connect();
    private:
        bool connected = false;
    };

    void GetAdaptersAddresses(std::vector<GetAdaptersAddressesResult>* result);
};
