#pragma once
#include <string>
#include <vector>

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
    void GetAdaptersInfo(std::string windows_LUID, std::vector<std::string>* result);
};
