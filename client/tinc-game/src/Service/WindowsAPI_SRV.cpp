// winsock2.h first: iphlpapi.h need it, and it refuse to load after windows.h
#include <winsock2.h>
#include <iphlpapi.h>
// Link with Iphlpapi.lib
#pragma comment(lib, "IPHLPAPI.lib")

#include <vector>
#include "WindowsAPI_SRV.h"

#include <setupapi.h>
#include <initguid.h>
#include <devguid.h>
#include <cwctype>

// Link with Setupapi.lib
#pragma comment(lib, "Setupapi.lib")

#include "wx/wx.h"

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

// Device setup class "Net" of network adapter, documented in
// https://learn.microsoft.com/en-us/windows-hardware/drivers/install/system-defined-device-setup-classes-available-to-vendors
// It is not localized, the GUID is the same on Windows XP and Windows 11
const GUID WindowsAPI_SRV::netDeviceClassGuid = GUID_DEVCLASS_NET;

namespace
{
    const DWORD SetupApiBufferCharacters = 512;
    const DWORD SetupApiMaxBufferCharacters = 64 * 1024;

    // Read device instance path, that is what tapinstall print on left of ": ",
    // this string is also what tapinstall remove take as argument
    bool ReadDeviceInstancePath(HDEVINFO deviceInfoSet, PSP_DEVINFO_DATA deviceInfoData, std::wstring* result)
    {
        DWORD requiredSize = 0;
        std::vector<wchar_t> buffer(SetupApiBufferCharacters);

        for (int attempt = 0; attempt < 4; attempt++) {
            BOOL ok = SetupDiGetDeviceInstanceIdW(deviceInfoSet, deviceInfoData,
                buffer.data(), (DWORD)buffer.size(), &requiredSize);
            if (ok) {
                result->assign(buffer.data());
                return true;
            }

            if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
                return false;
            }
            if (requiredSize > SetupApiMaxBufferCharacters) {
                return false;
            }
            buffer.resize(requiredSize + 1);
        }
        return false;
    }

    // Read NetCfgInstanceId of network device, the "{GUID}" also
    // GetAdaptersAddresses report as AdapterName
    bool ReadNetCfgInstanceId(HDEVINFO deviceInfoSet, PSP_DEVINFO_DATA deviceInfoData, std::wstring* result)
    {
        HKEY netCfgKey = SetupDiOpenDevRegKey(deviceInfoSet, deviceInfoData,
            DICS_FLAG_GLOBAL, 0, DIREG_DRV, KEY_READ);
        if (netCfgKey == INVALID_HANDLE_VALUE) {
            return false;
        }

        wchar_t buffer[256] = {};
        DWORD bufferSize = sizeof(buffer) - sizeof(wchar_t);
        DWORD valueType = 0;
        LONG status = RegQueryValueExW(netCfgKey, L"NetCfgInstanceId", NULL, &valueType,
            (LPBYTE)buffer, &bufferSize);
        RegCloseKey(netCfgKey);

        if (status != ERROR_SUCCESS || valueType != REG_SZ) {
            return false;
        }
        buffer[sizeof(buffer) / sizeof(wchar_t) - 1] = L'\0';
        result->assign(buffer);
        return true;
    }

    // Find device instance path in one device setup class,
    // match by device NetCfgInstanceId equal to netCfgInstanceId
    ReturnValue<std::wstring> FindDeviceInstancePathByNetCfgInstanceId(
        const GUID& classGuid, const std::wstring& netCfgInstanceId)
    {
        auto result = ReturnValue<std::wstring>();

        HDEVINFO deviceInfoSet = SetupDiGetClassDevsW(&classGuid, NULL, NULL, DIGCF_PRESENT);
        if (deviceInfoSet == INVALID_HANDLE_VALUE) {
            return result;
        }

        SP_DEVINFO_DATA deviceInfoData;
        deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

        for (DWORD deviceIndex = 0; SetupDiEnumDeviceInfo(deviceInfoSet, deviceIndex, &deviceInfoData); deviceIndex++) {
            std::wstring deviceNetCfgInstanceId;
            if (ReadNetCfgInstanceId(deviceInfoSet, &deviceInfoData, &deviceNetCfgInstanceId) == false) {
                continue;
            }

            if (WindowsAPI_SRV::IsSameNetCfgInstanceId(deviceNetCfgInstanceId, netCfgInstanceId) == false) {
                continue;
            }

            // DIGCF_PRESENT above already filter unreachable device, no status check needed here.
            // CM_Get_DevNode_Status do not help: status bits do NOT tell phantom, 0x00800000 is
            // DN_NT_ENUMERATOR and every root enumerated TAP carry it.
            // A real phantom check need DEVPKEY_Device_IsPresent, that do not exist before Vista
            if (ReadDeviceInstancePath(deviceInfoSet, &deviceInfoData, &result.returnBody) == false) {
                continue;
            }

            result.success = true;
            break;
        }

        SetupDiDestroyDeviceInfoList(deviceInfoSet);
        return result;
    }
}

bool WindowsAPI_SRV::IsSameNetCfgInstanceId(const std::wstring& left, const std::wstring& right)
{
    // Both "{76CCD955-AE88-4B48-BBF5-94075ABB2914}" but be tolerant to one without brace
    auto normalize = [](const std::wstring& input) {
        std::wstring trimmed = input;
        while (trimmed.empty() == false && std::iswspace(trimmed.front())) {
            trimmed.erase(trimmed.begin());
        }
        while (trimmed.empty() == false && std::iswspace(trimmed.back())) {
            trimmed.pop_back();
        }
        if (trimmed.size() >= 2 && trimmed.front() == L'{' && trimmed.back() == L'}') {
            trimmed = trimmed.substr(1, trimmed.size() - 2);
        }
        return trimmed;
        };

    return _wcsicmp(normalize(left).c_str(), normalize(right).c_str()) == 0;
}

ReturnValue<std::wstring> WindowsAPI_SRV::GetDeviceInstancePathByNetCfgInstanceId(std::string windows_LUID)
{
    // windows_LUID is the "AdapterName" of GetAdaptersAddresses, narrow to NetCfgInstanceId
    auto netCfgInstanceId = std::wstring(windows_LUID.begin(), windows_LUID.end());
    return FindDeviceInstancePathByNetCfgInstanceId(netDeviceClassGuid, netCfgInstanceId);
}

void WindowsAPI_SRV::GetAdaptersAddresses(std::vector<GetAdaptersAddressesResult>* result)
{
    /* Declare and initialize variables */

    DWORD dwRetVal = 0;

    unsigned int i = 0;

    // Set the flags to pass to GetAdaptersAddresses
    ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

    // default to unspecified address family (both)
    ULONG family = AF_UNSPEC;

    LPVOID lpMsgBuf = NULL;

    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    ULONG outBufLen = 0;
    ULONG Iterations = 0;

    PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
    //PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
    //PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
    //PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
    //IP_ADAPTER_DNS_SERVER_ADDRESS* pDnServer = NULL;
    //IP_ADAPTER_PREFIX* pPrefix = NULL;

    family = AF_INET;

    // Allocate a 15 KB buffer to start with.
    const int WORKING_BUFFER_SIZE = 15000;
    outBufLen = WORKING_BUFFER_SIZE;

    const int MAX_TRIES = 3;
    do {

        pAddresses = (IP_ADAPTER_ADDRESSES*)MALLOC(outBufLen);
        if (pAddresses == NULL) {
            printf
            ("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
            exit(1);
        }

        dwRetVal =
            GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

        if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
            FREE(pAddresses);
            pAddresses = NULL;
        }
        else {
            break;
        }

        Iterations++;

    } while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

    if (dwRetVal == NO_ERROR) {
        // If successful, output some information from the data we received
        pCurrAddresses = pAddresses;
        while (pCurrAddresses) {
            //printf("\tLength of the IP_ADAPTER_ADDRESS struct: %ld\n",
            //    pCurrAddresses->Length);
            //printf("\tIfIndex (IPv4 interface): %u\n", pCurrAddresses->IfIndex);
            //printf("\tAdapter name: %s\n", pCurrAddresses->AdapterName);

            //pUnicast = pCurrAddresses->FirstUnicastAddress;
            //if (pUnicast != NULL) {
            //    for (i = 0; pUnicast != NULL; i++)
            //        pUnicast = pUnicast->Next;
            //    printf("\tNumber of Unicast Addresses: %d\n", i);
            //}
            //else
            //    printf("\tNo Unicast Addresses\n");

            //pAnycast = pCurrAddresses->FirstAnycastAddress;
            //if (pAnycast) {
            //    for (i = 0; pAnycast != NULL; i++)
            //        pAnycast = pAnycast->Next;
            //    printf("\tNumber of Anycast Addresses: %d\n", i);
            //}
            //else
            //    printf("\tNo Anycast Addresses\n");

            //pMulticast = pCurrAddresses->FirstMulticastAddress;
            //if (pMulticast) {
            //    for (i = 0; pMulticast != NULL; i++)
            //        pMulticast = pMulticast->Next;
            //    printf("\tNumber of Multicast Addresses: %d\n", i);
            //}
            //else
            //    printf("\tNo Multicast Addresses\n");

            //pDnServer = pCurrAddresses->FirstDnsServerAddress;
            //if (pDnServer) {
            //    for (i = 0; pDnServer != NULL; i++)
            //        pDnServer = pDnServer->Next;
            //    printf("\tNumber of DNS Server Addresses: %d\n", i);
            //}
            //else
            //    printf("\tNo DNS Server Addresses\n");

            //printf("\tDNS Suffix: %wS\n", pCurrAddresses->DnsSuffix);
            //printf("\tDescription: %wS\n", pCurrAddresses->Description);
            //printf("\tFriendly name: %wS\n", pCurrAddresses->FriendlyName);

            auto networkAdapter = GetAdaptersAddressesResult();
            networkAdapter.friendlyName = pCurrAddresses->FriendlyName;
            networkAdapter.modelName = pCurrAddresses->Description;
            networkAdapter.windows_LUID = pCurrAddresses->AdapterName;
            result->push_back(networkAdapter);

            //if (pCurrAddresses->PhysicalAddressLength != 0) {
            //    printf("\tPhysical address: ");
            //    for (i = 0; i < (int)pCurrAddresses->PhysicalAddressLength;
            //        i++) {
            //        if (i == (pCurrAddresses->PhysicalAddressLength - 1))
            //            printf("%.2X\n",
            //                (int)pCurrAddresses->PhysicalAddress[i]);
            //        else
            //            printf("%.2X-",
            //                (int)pCurrAddresses->PhysicalAddress[i]);
            //    }
            //}
            //printf("\tFlags: %ld\n", pCurrAddresses->Flags);
            //printf("\tMtu: %lu\n", pCurrAddresses->Mtu);
            //printf("\tIfType: %ld\n", pCurrAddresses->IfType);
            //printf("\tOperStatus: %ld\n", pCurrAddresses->OperStatus);
            //printf("\tIpv6IfIndex (IPv6 interface): %u\n",
            //    pCurrAddresses->Ipv6IfIndex);
            //printf("\tZoneIndices (hex): ");
            //for (i = 0; i < 16; i++)
            //    printf("%lx ", pCurrAddresses->ZoneIndices[i]);
            //printf("\n");

            //printf("\tTransmit link speed: %I64u\n", pCurrAddresses->TransmitLinkSpeed);
            //printf("\tReceive link speed: %I64u\n", pCurrAddresses->ReceiveLinkSpeed);

            //pPrefix = pCurrAddresses->FirstPrefix;
            //if (pPrefix) {
            //    for (i = 0; pPrefix != NULL; i++)
            //        pPrefix = pPrefix->Next;
            //    printf("\tNumber of IP Adapter Prefix entries: %d\n", i);
            //}
            //else
            //    printf("\tNumber of IP Adapter Prefix entries: 0\n");

            //printf("\n");

            pCurrAddresses = pCurrAddresses->Next;
        }
    }
    else {
        //printf("Call to GetAdaptersAddresses failed with error: %d\n",
        //    dwRetVal);
        if (dwRetVal == ERROR_NO_DATA) {
            //printf("\tNo addresses were found for the requested parameters\n");
        }
        else {

            if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                // Default language
                (LPTSTR)&lpMsgBuf, 0, NULL)) {
                //printf("\tError: %s", lpMsgBuf);
                LocalFree(lpMsgBuf);
                if (pAddresses)
                    FREE(pAddresses);
                exit(1);
            }
        }
    }

    if (pAddresses) {
        FREE(pAddresses);
    }
}

void WindowsAPI_SRV::GetAdaptersInfo(std::string windows_LUID, std::vector<std::string>* result)
{
    IP_ADAPTER_INFO* pAdapterInfo;
    ULONG            ulOutBufLen;
    DWORD            dwRetVal;

    pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
    ulOutBufLen = sizeof(IP_ADAPTER_INFO);

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS) {
        free(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) != ERROR_SUCCESS) {
        //printf("GetAdaptersInfo call failed with %d\n", dwRetVal);
    }

    PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
    while (pAdapter) {
        //printf("Adapter Name: %s\n", pAdapter->AdapterName);
        //printf("Adapter Desc: %s\n", pAdapter->Description);
        //printf("\tAdapter Addr: \t");
        //for (UINT i = 0; i < pAdapter->AddressLength; i++) {
        //    if (i == (pAdapter->AddressLength - 1))
        //        printf("%.2X\n", (int)pAdapter->Address[i]);
        //    else
        //        printf("%.2X-", (int)pAdapter->Address[i]);
        //}
        //printf("IP Address: %s\n", pAdapter->IpAddressList.IpAddress.String);

        //// Iterate through the IP addresses
        //PIP_ADDR_STRING pAddress = &(pAdapter->IpAddressList);
        //while (pAddress) {
        //    std::string address(pAddress->IpAddress.String);
        //    pAddress = pAddress->Next;
        //}

        //printf("IP Mask: %s\n", pAdapter->IpAddressList.IpMask.String);
        //printf("\tGateway: \t%s\n", pAdapter->GatewayList.IpAddress.String);
        //printf("\t***\n");
        //if (pAdapter->DhcpEnabled) {
        //    printf("\tDHCP Enabled: Yes\n");
        //    printf("\t\tDHCP Server: \t%s\n", pAdapter->DhcpServer.IpAddress.String);
        //}
        //else
        //    printf("\tDHCP Enabled: No\n");

        if (windows_LUID == pAdapter->AdapterName) {
            PIP_ADDR_STRING pAddress = &(pAdapter->IpAddressList);
            while (pAddress) {
                result->push_back(pAddress->IpAddress.String);
                pAddress = pAddress->Next;
            }

            break;
        }

        pAdapter = pAdapter->Next;
    }

    if (pAdapterInfo)
        free(pAdapterInfo);
}

bool WindowsAPI_SRV::GetAdaptersAddressesResult::IsLoopback()
{
    auto isLoopback = this->modelName.find(L"Software Loopback") != std::wstring::npos;
    return isLoopback;
}

bool WindowsAPI_SRV::GetAdaptersAddressesResult::IsTap()
{
    bool isTap = this->modelName.find(L"TAP-Win32 Adapter V9") == 0;
    bool containsHash = this->modelName.find(L"#") != std::wstring::npos;
    if (containsHash) {
        isTap = isTap && this->modelName[21] == L'#';
    }
    return isTap;
}

bool WindowsAPI_SRV::GetAdaptersAddressesResult::Available()
{
    return !connected;
}

bool WindowsAPI_SRV::GetAdaptersAddressesResult::Connect()
{
    if (connected) {
        return false;
    }

    connected = true;
    return true;
}

void WindowsAPI_SRV::GetAdaptersAddressesResult::Disconnect()
{
    connected = false;
}
