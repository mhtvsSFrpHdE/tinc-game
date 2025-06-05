#include "boost/process.hpp"
#include <boost/process/windows.hpp>
#include "MainFrame.h"
#include "String_SRV.h"
#include <sstream>
#include <wx/filename.h>
#include <boost/circular_buffer.hpp>

void MainFrame::API_SRV_ConnectToNetwork(PerNetworkData* perNetworkData)
{
    auto result = ReturnValue<ConnectToNetworkResult>();

    namespace bp = boost::process;
    namespace sr = String_SRV;
    bp::ipstream is;

    wxFileName pid;
    pid.SetPath(perNetworkData->network.GetFullPath());
    pid.SetName(L"pid");
    if (pid.Exists()) {
        auto disconnectResult = API_SRV_DisconnectNetwork(perNetworkData);
        if (disconnectResult.success == false) {
            result.returnBody.messageEnum = ConnectToNetworkResult::Enum::TincNotExist;
            CallAfter(&MainFrame::API_UI_EndConnectToNetwork, result, perNetworkData);
            return;
        }
    }

    wxFileName tincBin;
    tincBin.AppendDir("bin");
    tincBin.AppendDir("tinc");
    tincBin.SetName(L"tincd.exe");
    auto tincBinPath = tincBin.GetFullPath();

    std::wstringstream commandStringStream;
    commandStringStream << tincBinPath
        << sr::space << L"--config="
        << sr::doubleQuotes << perNetworkData->network.GetFullPath() << sr::doubleQuotes
        << sr::space << L"--option=interface="
        << sr::doubleQuotes << perNetworkData->tap->friendlyName << sr::doubleQuotes
        << sr::space << L"--no-detach"
        << sr::space << L"--debug=3";

    auto command = commandStringStream.str();

    boost::circular_buffer<std::string> cb(10);
    try {
        perNetworkData->tincProcess = std::shared_ptr<bp::child>(new bp::child(command, bp::std_err > is, bp::windows::hide));
        CallAfter(&MainFrame::API_UI_SetDisconnectButtonEnable, true, perNetworkData->disconnectButton);

        std::string line;
        while (std::getline(is, line)) {
            cb.push_back(line);
            auto wline = sr::ForceToWstring(line);
            CallAfter(&MainFrame::API_UI_ReportStatus, wline, perNetworkData->liveLog);

            // {3D6DCCE4-F183-4859-9B35-129361209E5F} (Ethernet 2) is not a usable Windows tap device: (31) A device attached to the system is not functioning.

            // Ready
            if (line == "Ready\r") {
                result.success = true;
            }
        }
        perNetworkData->tincProcess->wait();
    }
    catch (...) {
        CallAfter(&MainFrame::API_UI_SetDisconnectButtonEnable, false, perNetworkData->disconnectButton);

        result.returnBody.messageEnum = ConnectToNetworkResult::Enum::TincNotExist;
        CallAfter(&MainFrame::API_UI_EndConnectToNetwork, result, perNetworkData);

        return;
    }
    CallAfter(&MainFrame::API_UI_SetDisconnectButtonEnable, false, perNetworkData->disconnectButton);

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

ReturnValue<std::wstring> MainFrame::API_SRV_DisconnectNetwork(PerNetworkData* perNetworkData)
{
    auto result = ReturnValue<std::wstring>();

    namespace ss = String_SRV;
    namespace bp = boost::process;

    wxFileName tincBin;
    tincBin.AppendDir("bin");
    tincBin.AppendDir("tinc");
    tincBin.SetName(L"tinc.exe");
    auto tincBinPath = tincBin.GetFullPath();

    std::wstringstream commandStringStream;
    commandStringStream << tincBinPath
        << ss::space << L"--config="
        << ss::doubleQuotes << perNetworkData->network.GetFullPath() << ss::doubleQuotes
        << ss::space << L"stop";
    auto command = commandStringStream.str();

    std::wostringstream lines;
    try {
        bp::ipstream is;
        std::string line;
        bp::child c(command, bp::std_err > is, bp::windows::hide);
        while (std::getline(is, line)) {
            lines << line;
        }
        c.wait();

        auto collectOutput = lines.str();
        result.returnBody = collectOutput;
        CallAfter(&MainFrame::API_UI_ReportStatus, collectOutput, perNetworkData->liveLog);
    }
    catch (...) {
        return result;
    }

    result.success = true;
    return result;
}

void MainFrame::API_UI_EndConnectToNetwork(ReturnValue<ConnectToNetworkResult> result, PerNetworkData* perNetworkData)
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
        if (result.returnBody.messageEnum == ConnectToNetworkResult::Enum::TincNotExist) {
            errorMessage << _("Installation corroupted, tinc.exe or tincd.exe may not exist") << std::endl;
        }
        if (result.returnBody.messageEnum == ConnectToNetworkResult::Enum::Other) {
            errorMessage << _("Unknown error:") << std::endl
                << result.returnBody.messageString;
        }

        wxMessageDialog(this, errorMessage.str()).ShowModal();

        perNetworkData->tap->Disconnect();
        UpdateCurrentTapItemDisplayText(*perNetworkData->tap, perNetworkData->tapSelection);
        perNetworkData->disconnectButton->Enable(true);
        return;
    }

    perNetworkData->tap->Disconnect();
    UpdateCurrentTapItemDisplayText(*perNetworkData->tap, perNetworkData->tapSelection);
    perNetworkData->connectButton->Enable(true);
}
