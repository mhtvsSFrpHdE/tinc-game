#include "boost/process.hpp"
#include <boost/process/windows.hpp>
#include "MainFrame.h"
#include "String_SRV.h"
#include <sstream>
#include <boost/circular_buffer.hpp>
#include "Resource_SRV.h"
#include "Settings_SRV.h"
#include "Lock_SRV.h"

void MainFrame::API_SRV_ConnectToNetwork(PerNetworkData* perNetworkData)
{
    auto result = ReturnValue<ConnectToNetworkResult>();

    namespace bp = boost::process;
    namespace sr = String_SRV;
    namespace rst = Resource_SRV::TincBin;
    namespace rsb = Resource_SRV::Bat;
    namespace ss = Settings_SRV;

    auto setMetricSettingsKey = SettingKeys_Networks::network_setMetric(perNetworkData->network.networkName);
    auto setMetric = ss::networksConfig->ReadBool(setMetricSettingsKey, true);
    if (setMetric) {
        // netsh interface ipv4 set interface "interface_name" metric=value
        bp::system(bp::shell(), bp::args({ rsb::cmdRumCommand, rsb::netsh, L"interface", rsb::netshArgV4, L"set", L"interface", perNetworkData->tap->friendlyName, L"metric=1" }), bp::windows::hide);
        bp::system(bp::shell(), bp::args({ rsb::cmdRumCommand, rsb::netsh, L"interface", rsb::netshArgV6, L"set", L"interface", perNetworkData->tap->friendlyName, L"metric=1" }), bp::windows::hide);
    }

    bp::ipstream is;

    auto pid = rst::GetTincdPid(perNetworkData->network);
    if (pid.Exists()) {
        auto disconnectResult = API_SRV_DisconnectNetwork(perNetworkData);
        if (disconnectResult.success == false) {
            result.returnBody.messageEnum = ConnectToNetworkResult::Enum::TincNotExist;
            CallAfter(&MainFrame::API_UI_EndConnectToNetwork, result, perNetworkData);
            return;
        }
    }

    auto verboseSettingsKey = SettingKeys_Networks::network_verbose(perNetworkData->network.networkName);
    auto verbose = ss::networksConfig->ReadBool(verboseSettingsKey, true);

    auto portSettingsKey = SettingKeys_Networks::network_port(perNetworkData->network.networkName);
    auto port = ss::networksConfig->Read(portSettingsKey);

    wxString tincBinPath;
    auto networkGameModeSettingsKey = SettingKeys_Networks::network_gameMode(perNetworkData->network.networkName);
    auto gameMode = ss::networksConfig->ReadBool(networkGameModeSettingsKey, false);
    if (gameMode) {
        auto programGameModeSettingsKey = SettingKeys_Program::settings_gameMode;
        auto gameModeValue = ss::programConfig->Read(programGameModeSettingsKey);
        auto tincGameModeBin = rst::GetTincdGameModeBin(gameModeValue);
        tincBinPath = tincGameModeBin.GetFullPath();

        bool gameModeFileExists = wxFileExists(tincBinPath);
        if (gameModeFileExists == false) {
            auto srcTincdPath = rst::GetTincdBinAsWxStr();
            auto dstTincdPath = tincGameModeBin.GetFullPath();
            wxCopyFile(srcTincdPath, dstTincdPath);
        }
    }
    else {
        tincBinPath = rst::GetTincdBinAsWxStr();
    }

    std::wstringstream commandStringStream;
    commandStringStream << tincBinPath
        << sr::space << L"--config="
        << sr::doubleQuotes << perNetworkData->network.GetFullPath() << sr::doubleQuotes
        << sr::space << L"--option=interface="
        << sr::doubleQuotes << perNetworkData->tap->friendlyName << sr::doubleQuotes
        << sr::space << L"--option=port=" << port
        << sr::space << L"--no-detach";
    if (verbose) {
        commandStringStream << sr::space << L"--debug=3";
    }

    auto command = commandStringStream.str();

    boost::circular_buffer<std::string> cb(10);
    try {
        perNetworkData->tincProcess = std::shared_ptr<bp::child>(new bp::child(command, bp::std_err > is, bp::windows::hide));
        CallAfter(&MainFrame::API_UI_SetDisconnectButtonEnable, true, perNetworkData->disconnectButton.get());

        std::string line;
        while (std::getline(is, line)) {
            cb.push_back(line);
            auto wline = sr::ForceToWstring(line);
            CallAfter(&MainFrame::API_UI_ReportStatus, wline, perNetworkData->liveLog.get());

            // {3D6DCCE4-F183-4859-9B35-129361209E5F} (Ethernet 2) is not a usable Windows tap device: (31) A device attached to the system is not functioning.

            // Ready
            if (line == "Ready\r") {
                result.success = true;
            }
        }
        perNetworkData->tincProcess->wait();
    }
    catch (...) {
        CallAfter(&MainFrame::API_UI_SetDisconnectButtonEnable, false, perNetworkData->disconnectButton.get());

        result.returnBody.messageEnum = ConnectToNetworkResult::Enum::TincNotExist;
        CallAfter(&MainFrame::API_UI_EndConnectToNetwork, result, perNetworkData);

        return;
    }
    CallAfter(&MainFrame::API_UI_SetDisconnectButtonEnable, false, perNetworkData->disconnectButton.get());

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
    namespace rst = Resource_SRV::TincBin;

    auto tincBinPath = rst::GetTincBinAsWxStr();
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
        CallAfter(&MainFrame::API_UI_ReportStatus, collectOutput, perNetworkData->liveLog.get());
    }
    catch (...) {
        return result;
    }

    result.success = true;
    return result;
}

void MainFrame::API_SRV_PostLayout()
{
    namespace ss = String_SRV;
    {
        int pendingAutoStartCount = autoStartNetworkRawDataIndex_pending.size();
        if (pendingAutoStartCount > 0) {
            for (int i = 0; i < pendingAutoStartCount; i++)
            {
                auto autoStartIndex = autoStartNetworkRawDataIndex_pending[i];
                auto& perNetworkData = currentNetwork_ComboBox_RawData[autoStartIndex];

                CallAfter([this, autoStartIndex] {
                    currentNetwork_ComboBox->SetSelection(autoStartIndex);
                    currentNetwork_ComboBox->SendSelectionChangedEvent(wxEVT_COMBOBOX);
                    });
                CallAfter([this] {
                    Lock_SRV::Notify(uiMutex, uiCb);
                    });
                Lock_SRV::Wait(uiMutex, uiCb);

                auto tapSelection = currentTap_ComboBox->GetSelection();
                if (tapSelection == wxNOT_FOUND) {
                    auto hint = _("Network ") + perNetworkData->network.networkName + _(" was set to auto connect on start")
                        + ss::newLine + _("but there is no available virtual network adapter");
                    wxMessageDialog(this, hint).ShowModal();
                    continue;
                }
                autoStartNetworkRawDataIndex_submitted.push_back(autoStartIndex);
                CallAfter(&MainFrame::OnConnectButtonClick_Internal);
            }
        }

        int submittedAutoStartCount = autoStartNetworkRawDataIndex_submitted.size();
        std::wostringstream submittedNetworkSstream;
        if (pendingAutoStartCount != submittedAutoStartCount) {
            for (int i = 0; i < submittedAutoStartCount; i++)
            {
                auto submittedAutoStartIndex = autoStartNetworkRawDataIndex_submitted[i];
                auto& perNetworkData = currentNetwork_ComboBox_RawData[submittedAutoStartIndex];
                submittedNetworkSstream << perNetworkData->network.networkName << ss::newLine;
            }
            auto submittedNetworks = submittedNetworkSstream.str();
            submittedNetworks = submittedNetworks.substr(0, submittedNetworks.length() - 1);
            auto hint = _("Not all auto connect network submitted") + ss::newLine + _("The following network successfully submitted to auto connected, you may want to disconnect them before edit settings") + ss::newLine + submittedNetworks;
            CallAfter([this, hint]() {
                wxMessageDialog(this, hint).ShowModal();
                });
        }

        autoStartNetworkRawDataIndex_pending.clear();
        autoStartNetworkRawDataIndex_submitted.clear();
    }

    {
        CallAfter([this]() {
            currentNetwork_ComboBox->SetSelection(recentUsedNetworkSelection);
            currentNetwork_ComboBox->SendSelectionChangedEvent(wxEVT_COMBOBOX);
            Enable(true);
            SetFocus();
            });
    }
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
        allowCloseFrame = true;
        return;
    }

    perNetworkData->tap->Disconnect();
    UpdateCurrentTapItemDisplayText(*perNetworkData->tap, perNetworkData->tapSelection);
    OnNetworkDisconnected(perNetworkData);
}
