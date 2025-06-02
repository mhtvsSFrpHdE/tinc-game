#include "boost/process.hpp"
#include <boost/process/windows.hpp>
#include "MainFrame.h"
#include "String_SRV.h"
#include <sstream>
#include <wx/filename.h>
#include <boost/circular_buffer.hpp>

void MainFrame::API_SRV_ConnectToNetwork(PerNetworkData perNetworkData)
{
    auto result = ReturnValue<ConnectToNetworkResult>();

    namespace bp = boost::process;
    namespace sr = String_SRV;
    bp::ipstream is;
    bp::opstream in;

    wxFileName tincBin;
    tincBin.AppendDir("bin");
    tincBin.AppendDir("tinc");
    tincBin.SetName(L"tincd.exe");
    auto tincBinPath = tincBin.GetFullPath();

    std::wstringstream commandStringStream;
    commandStringStream << tincBinPath
        << sr::space << L"--config="
        << sr::doubleQuotes << perNetworkData.network.GetFullPath() << sr::doubleQuotes
        << sr::space << L"--option=interface="
        << sr::doubleQuotes << perNetworkData.tap.get()->friendlyName << sr::doubleQuotes
        << sr::space << L"--no-detach"
        << sr::space << L"--debug=3";

    auto command = commandStringStream.str();

    boost::circular_buffer<std::string> cb(10);
    try {
        perNetworkData.tincProcess = std::shared_ptr<bp::child>(new bp::child(command, bp::std_err > is, bp::std_in < in));
        //bp::child c(command, bp::std_err > is, bp::windows::hide);

        std::string line;
        while (std::getline(is, line)) {
            cb.push_back(line);
            auto wline = sr::ForceToWstring(line);
            CallAfter(&MainFrame::API_UI_ReportStatus, wline, perNetworkData.liveLog);

            // {3D6DCCE4-F183-4859-9B35-129361209E5F} (Ethernet 2) is not a usable Windows tap device: (31) A device attached to the system is not functioning.

            // Ready
            if (line == "Ready\r") {
                result.success = true;
            }
        }
        perNetworkData.tincProcess->wait();
    }
    catch (...) {
        result.returnBody.messageEnum = ConnectToNetworkResult::Enum::TincdNotExist;
        CallAfter(&MainFrame::API_UI_EndConnectToNetwork, result, perNetworkData);

        return;
    }

    if (result.success == false) {
        std::wstringstream messageStringStream;
        for (auto& line : cb) {
            messageStringStream << line;
        }
        result.returnBody.messageEnum = ConnectToNetworkResult::Enum::Other;
        result.returnBody.messageString = messageStringStream.str();
        CallAfter(&MainFrame::API_UI_EndConnectToNetwork, result, perNetworkData);
        return;
    }

    CallAfter(&MainFrame::API_UI_EndConnectToNetwork, result, perNetworkData);
    return;
}

void MainFrame::API_UI_EndConnectToNetwork(ReturnValue<ConnectToNetworkResult> result, PerNetworkData perNetworkData)
{
    if (result.success == false) {
        std::wstringstream errorMessage;
        errorMessage << _("Failed to connect to network ") << result.returnBody.network.networkName << ":" << std::endl;

        if (result.returnBody.messageEnum == ConnectToNetworkResult::Enum::TapUnavailable) {
            errorMessage << _("Virtual network adapter ") << result.returnBody.tap.friendlyName << _(" Unavailable");
        }
        if (result.returnBody.messageEnum == ConnectToNetworkResult::Enum::RefusedByTinc) {
            errorMessage << _("Connect request refused by tinc:") << std::endl
                << result.returnBody.messageString;
        }
        if (result.returnBody.messageEnum == ConnectToNetworkResult::Enum::Other) {
            errorMessage << _("Unknown error:") << std::endl
                << result.returnBody.messageString;
        }

        wxMessageDialog(this, errorMessage.str()).ShowModal();

        perNetworkData.tap.get()->Disconnect();
        UpdateCurrentTapItemDisplayText(*perNetworkData.tap.get(), perNetworkData.tapSelection);
        perNetworkData.connectButton->Enable(true);
    }

    perNetworkData.tap.get()->Disconnect();
    UpdateCurrentTapItemDisplayText(*perNetworkData.tap.get(), perNetworkData.tapSelection);
    perNetworkData.connectButton->Enable(true);
}
