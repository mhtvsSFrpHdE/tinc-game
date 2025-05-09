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

ReturnValue<std::vector<std::wstring>> ApplyMtuFrame::API_SRV_GetNetworkAdapterList()
{
    // TODO
    // Return false if network adapter list has 0 available network adapter
    // Return true if any available network adapter founded
    return ReturnValue<std::vector<std::wstring>>();
}

ReturnValue<ApplyMtuResult> ApplyMtuFrame::API_SRV_ApplyMtu(int mtu_IPv4, int mtu_IPv6, std::wstring adapterName)
{
    auto result = ReturnValue<ApplyMtuResult>();
    namespace bp = boost::process;
    bp::ipstream is;

    bp::child c(std::wstring(L"netsh437.bat "), adapterName, std::to_wstring(mtu_IPv4), "ipv4", bp::std_out > is, bp::windows::hide);
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
    bp::child d(std::wstring(L"netsh437.bat"), adapterName, std::to_string(mtu_IPv6), "ipv6", bp::std_out > is2, bp::windows::hide);
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
