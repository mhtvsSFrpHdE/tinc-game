#include "boost/process.hpp"
#include <boost/process/windows.hpp>
#include "ApplyMtuFrame.h"
#include "String_SRV.h"
#include <codecvt>
#include <sstream>
#include <thread>
#include <vector>
#include <string> 

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
            result.returnBody.messageEnum = ApplyMtuResult::Enum::InvalidAdapterName;
            break;
        }
        if (result.success == false) {
            result.returnBody.messageEnum = ApplyMtuResult::Enum::Other;
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
        << sr::space << mtu_IPv6
        << sr::space << L"ipv6";
    auto netshCommandd = commandStringStream.str();
    bp::child d(bp::shell(), bp::args({ L"/c", netshCommandd }), bp::std_out > is2, bp::windows::hide);
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
            result.returnBody.messageEnum = ApplyMtuResult::Enum::Failed_IPv6;
            break;
        }

        if (result.success == false) {
            result.returnBody.messageEnum = ApplyMtuResult::Enum::Other;
            result.returnBody.messageString = wline;
            break;
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
