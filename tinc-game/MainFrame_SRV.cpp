#include "boost/process.hpp"
#include <boost/process/windows.hpp>
#include "MainFrame.h"
#include "String_SRV.h"
#include <sstream>
#include <wx/filename.h>

ReturnValue<ConnectToNetworkResult> MainFrame::API_SRV_ConnectToNetwork(Networks_SRV::GetNetworksResult network, WindowsAPI_SRV::GetAdaptersAddressesResult tap)
{
    auto result = ReturnValue<ConnectToNetworkResult>();

    namespace bp = boost::process;
    namespace sr = String_SRV;
    bp::ipstream is;

    wxFileName tincBin;
    tincBin.AppendDir("bin");
    tincBin.AppendDir("tinc");
    tincBin.SetName(L"tincd.exe");
    auto tincBinPath = tincBin.GetFullPath();

    std::wstringstream commandStringStream;
    commandStringStream << tincBinPath
        << sr::space << L"--config="
        << sr::doubleQuotes << network.GetFullPath() << sr::doubleQuotes
        << sr::space << L"--option=interface="
        << sr::doubleQuotes << tap.friendlyName << sr::doubleQuotes
        << sr::space << L"--no-detach"
        << sr::space << L"--debug=3";
    auto command = commandStringStream.str();
    bp::child c(command, bp::std_out > is);

    std::string line;
    std::vector<std::string> lines;

    //while (std::getline(is, line)) {
    //    lines.push_back(line);
    //}

    c.wait();

    return result;
}
