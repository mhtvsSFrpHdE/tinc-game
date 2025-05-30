#pragma once
#include <vector>
#include <string>

namespace WindowsAPI_SRV
{
    struct GetAdaptersAddressesResult {
        std::wstring friendlyName;
        std::wstring modelName;
        std::string windows_LUID;
        bool IsLoopback();
        bool IsTap();
        bool Available();
        bool Connect();
        void Disconnect();
    private:
        bool connected = false;
    };

    void GetAdaptersAddresses(std::vector<GetAdaptersAddressesResult>* result);
};
