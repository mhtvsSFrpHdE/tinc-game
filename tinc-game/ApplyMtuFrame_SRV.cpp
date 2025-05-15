#include "boost/process.hpp"
#include <boost/process/windows.hpp>
#include "ApplyMtuFrame.h"
#include "String_SRV.h"
#include <codecvt>
#include "clip.h"
#pragma comment(lib, "clip.lib")
#include <sstream>
#include <thread>
#include <vector>
#include <string> 

#include <winsock2.h>
#include <iphlpapi.h>
// Link with Iphlpapi.lib
#pragma comment(lib, "IPHLPAPI.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

void API_Windows_GetAdaptersAddresses(std::vector<GetNetworkAdapterListResult>* result) {
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

            auto networkAdapter = GetNetworkAdapterListResult();
            networkAdapter.friendlyName = pCurrAddresses->FriendlyName;
            networkAdapter.modelName = pCurrAddresses->Description;
            networkAdapter.windows_LUID = pCurrAddresses->AdapterName;
            networkAdapter.isLoopback = wcsstr(pCurrAddresses->Description, L"Software Loopback");
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

ReturnValue<std::vector<GetNetworkAdapterListResult>> ApplyMtuFrame::API_SRV_GetNetworkAdapterList()
{
    auto result = ReturnValue<std::vector<GetNetworkAdapterListResult>>();

    API_Windows_GetAdaptersAddresses(&result.returnBody);

    if (result.returnBody.size() > 0) {
        result.success = true;
    }

    return result;
}

ReturnValue<ApplyMtuResult> ApplyMtuFrame::API_SRV_ApplyMtu(int mtu_IPv4, int mtu_IPv6, std::wstring adapterName)
{
    auto result = ReturnValue<ApplyMtuResult>();
    namespace bp = boost::process;
    namespace sr = String_SRV;
    bp::ipstream is;

    std::wstringstream commandStringStream;
    commandStringStream << L"netsh437.bat"
        << sr::space << sr::doubleQuotes << adapterName << sr::doubleQuotes
        << sr::space << mtu_IPv4
        << sr::space << L"ipv6";
    auto netshCommand = commandStringStream.str();
    bp::child c(bp::shell(), bp::args({ L"/c", netshCommand }), bp::std_out > is, bp::windows::hide);

    c.wait();
    std::string line;
    std::ostringstream s;
    while (std::getline(is, line)) {
        s << line;
        auto wline = String_SRV::ForceToWstring(s.str());
        // Ok.
        if (line.find(std::string("Ok")) != std::wstring::npos) {

            result.success = true;
            break;
        }
        // The filename, directory name, or volume label syntax is incorrect.
        if (line.find(std::string("name")) != std::wstring::npos) {
            result.success = false;
            result.returnBody.messageEnum = ApplyMtuResult::ApplyMtu_InvalidAdapterName;
            break;
        }
        if (result.success == false) {
            result.returnBody.messageEnum = ApplyMtuResult::ApplyMtu_Other;
            result.returnBody.messageString = wline;
            break;
        }
    }
    is.close();
    if (result.success == false) {
        return result;
    }
    s.str("");
    s.clear();
    bp::ipstream is2;

    commandStringStream.str(std::wstring());
    commandStringStream << L"netsh437.bat"
        << sr::space << sr::doubleQuotes << adapterName << sr::doubleQuotes
        << sr::space << mtu_IPv4
        << sr::space << L"ipv6";
    auto netshCommandd = commandStringStream.str();
    bp::child d(bp::shell(), bp::args({ L"/c", netshCommandd }), bp::std_out > is, bp::windows::hide);
    d.wait();
    std::string line2;
    while (std::getline(is2, line2)) {
        s << line2;
        auto wline = String_SRV::ForceToWstring(s.str());
        // Ok.
        if (line2.find("Ok") != std::wstring::npos) {
            break;
        }
        // Element not found.
        if (line2.find("Element") != std::wstring::npos) {
            result.success = false;
            result.returnBody.messageEnum = ApplyMtuResult::ApplyMtu_Failed_IPv6;
            break;
        }

        if (result.success == false) {
            result.returnBody.messageEnum = ApplyMtuResult::ApplyMtu_Other;
            result.returnBody.messageString = wline;
            break;
        }
    }

    return result;
}

bool ApplyMtuFrame::API_SRV_OpenNetworkControlPanel()
{
    using namespace boost::process;
    try {
        child c("control.exe ncpa.cpl");
        c.wait();
        return true;
    }
    catch (...) {
        return false;
    }
}

void ApplyMtuFrame::API_SRV_OpenCommandPrompt()
{
    namespace bp = boost::process;
    try {
        bp::child process("cmd.exe");
        process.detach();
    }
    catch (...) {
        return;
    }
}

std::wstring ApplyMtuFrame::API_SRV_GetNetshCommand(std::wstring adapterName, int mtu_IPv4, int mtu_IPv6)
{
    const std::wstring doubleQuotes = L"\"";

    std::wostringstream netshCommandStringBuilder;
    netshCommandStringBuilder
        << L"netsh.exe interface ipv4 set subinterface "
        << doubleQuotes << adapterName << doubleQuotes
        << L" mtu=" << std::to_wstring(mtu_IPv4)
        << L" store=persistent"
        << L" & "
        << L"netsh.exe interface ipv6 set subinterface "
        << doubleQuotes << adapterName << doubleQuotes
        << L" mtu=" << std::to_wstring(mtu_IPv6)
        << L" store=persistent";

    auto netshCommand = netshCommandStringBuilder.str();
    return netshCommand;
}

void ApplyMtuFrame::API_SRV_CopyNetshCommand(std::wstring command)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    auto commandBytes = conv.to_bytes(command);
    clip::set_text(commandBytes);
}
