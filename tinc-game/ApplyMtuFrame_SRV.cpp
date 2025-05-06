#include <boost/process.hpp>
#include "ApplyMtuFrame.h"
#include "String_SRV.h"
#include <codecvt>
#include "clip.h"
#pragma comment(lib, "clip.lib")

ReturnValue<std::vector<std::wstring>> ApplyMtuFrame::API_SRV_GetNetworkAdapterList()
{
    // TODO
    // Return false if network adapter list has 0 available network adapter
    // Return true if any available network adapter founded
    return ReturnValue<std::vector<std::wstring>>();
}

ReturnValue<ApplyMtuResult> ApplyMtuFrame::API_SRV_ApplyMtu(int mtu_IPv4, int mtu_IPv6, std::wstring adapterName)
{
    // TODO
    // Return false if "Apply MTU failed on adapter's IPv4", report error message from netsh
    // Return true if "All success", "IPv4 success but IPv6 failed"
    auto what = ReturnValue<ApplyMtuResult>();
    what.success = false;
    what.returnBody.messageEnum = ApplyMtuResult::ApplyMtu_Failed_IPv4;
    what.returnBody.messageString = L"";
    return ReturnValue<ApplyMtuResult>();
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
