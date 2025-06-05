#include "boost/process.hpp"
#include <boost/process/windows.hpp>
#include "ApplyMtuFrame.h"
#include "String_SRV.h"
#include <codecvt>
#include <sstream>
#include <thread>
#include <vector>
#include <string>
#include "Resource_SRV.h"

ReturnValue<ApplyMtuResult> ApplyMtuFrame::API_SRV_ApplyMtu(int mtu_IPv4, int mtu_IPv6, std::wstring adapterName)
{
    auto result = ReturnValue<ApplyMtuResult>();

    namespace bp = boost::process;
    namespace sr = String_SRV;
    namespace rsb = Resource_SRV::Bat;

    {
        bp::ipstream is;
        bp::child c(bp::shell(), bp::args({ rsb::cmdRumCommand, rsb::netsh437, rsb::netshArgV4, adapterName, std::to_wstring(mtu_IPv4) }), bp::std_out > is, bp::windows::hide);

        std::string line;
        std::wostringstream lines;
        while (std::getline(is, line)) {
            lines << line;

            // Ok.
            if (line.find(std::string("Ok")) != std::wstring::npos) {
                result.success = true;
                break;
            }
            // The filename, directory name, or volume label syntax is incorrect.
            if (line.find(std::string("name")) != std::wstring::npos) {
                result.success = false;
                result.returnBody.messageEnum = ApplyMtuResult::Enum::InvalidAdapterName;
                return result;
            }
        }
        c.wait();

        if (result.success == false) {
            result.returnBody.messageEnum = ApplyMtuResult::Enum::Other;
            result.returnBody.messageString = lines.str();
            return result;
        }
    }

    {
        bp::ipstream is;
        bp::child c(bp::shell(), bp::args({ rsb::cmdRumCommand, rsb::netsh437, rsb::netshArgV6, adapterName, std::to_wstring(mtu_IPv6) }), bp::std_out > is, bp::windows::hide);

        std::string line;
        std::wostringstream lines;
        while (std::getline(is, line)) {
            lines << line;

            // Ok.
            if (line.find("Ok") != std::wstring::npos) {
                return result;
            }
        }
        c.wait();

        if (result.success == false) {
            result.returnBody.messageEnum = ApplyMtuResult::Enum::Failed_IPv6;
            result.returnBody.messageString = lines.str();
            return result;
        }
    }

    return result;
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
