#include "MainFrame.h"
#include <wx/spinctrl.h>
#include <wx/dialog.h>
#include "Resource_SRV.h"
#include <sstream>
#include "String_SRV.h"
#include "Settings_SRV.h"
#include "Networks_SRV.h"
#include "TapDevice_SRV.h"

void MainFrame::API_UI_SetDisconnectStatus(bool enable, PerNetworkData* perNetworkData)
{
    perNetworkData->disconnectButton->Enable(enable);
    perNetworkData->ipCopyAndRefreshButton->Enable(enable);
    if (enable == false) {
        perNetworkData->ipTextCtrl->Clear();
    }
}

void MainFrame::API_UI_SetEditButtonEnable(bool enable, wxButton* editButton)
{
    editButton->Enable(enable);
}

void MainFrame::API_UI_ReportStatus(std::wstring status, tincTextCtrl* liveLog)
{
    liveLog->tincAppendText(status);
}

void MainFrame::OnCurrentNetworkChange(wxCommandEvent& evt)
{
    auto selection = currentNetwork_ComboBox->GetSelection();
    auto& rawData = currentNetwork_ComboBox_RawData[selection];

    {
        recentActiveLiveLog->Hide();
        liveLogSizer->Replace(recentActiveLiveLog.get(), rawData->liveLog.get());
        rawData->liveLog->Show();
        recentActiveLiveLog = rawData->liveLog;

        liveLogSizer->Layout();
    }

    {
        recentActiveConnectButton->Hide();
        networkControlNavigateSizer->Replace(recentActiveConnectButton.get(), rawData->connectButton.get());
        rawData->connectButton->Show();
        recentActiveConnectButton = rawData->connectButton;

        recentActiveDisconnectButton->Hide();
        networkControlNavigateSizer->Replace(recentActiveDisconnectButton.get(), rawData->disconnectButton.get());
        rawData->disconnectButton->Show();
        recentActiveDisconnectButton = rawData->disconnectButton;

        networkControlNavigateSizer->Layout();
    }

    {
        recentActiveIpCopyAndRefreshButton->Hide();
        networkControlIpSizer->Replace(recentActiveIpCopyAndRefreshButton.get(), rawData->ipCopyAndRefreshButton.get());
        rawData->ipCopyAndRefreshButton->Show();
        recentActiveIpCopyAndRefreshButton = rawData->ipCopyAndRefreshButton;

        recentActiveIpTextCtrl->Hide();
        networkControlIpSizer->Replace(recentActiveIpTextCtrl.get(), rawData->ipTextCtrl.get());
        rawData->ipTextCtrl->Show();
        recentActiveIpTextCtrl = rawData->ipTextCtrl;

        networkControlIpSizer->Layout();
    }

    {
        for (size_t i = 0; i < currentTap_ComboBox_RawData.size(); i++)
        {
            auto& adapter = currentTap_ComboBox_RawData[i];

            if (rawData->tap != nullptr) {
                if (adapter.friendlyName == rawData->tap->friendlyName) {
                    currentTap_ComboBox->SetSelection(i);
                    return;
                }
            }

            if (adapter.Available() && adapter.friendlyName == rawData->network.recentUsedTapName) {
                currentTap_ComboBox->SetSelection(i);
                return;
            }
        }
        for (size_t i = 0; i < currentTap_ComboBox_RawData.size(); i++) {
            auto& adapter = currentTap_ComboBox_RawData[i];

            if (adapter.Available()) {
                currentTap_ComboBox->SetSelection(i);
                return;
            }
        }
        currentTap_ComboBox->SetSelection(wxNOT_FOUND);
    }
}

wxString MainFrame::GetCurrentTapDisplayText(WindowsAPI_SRV::GetAdaptersAddressesResult tap)
{
    auto displayText = tap.friendlyName + " | " + (tap.Available() ? _("Available") : _("Connected"));
    return displayText;
}

void MainFrame::UpdateCurrentTapItemDisplayText(WindowsAPI_SRV::GetAdaptersAddressesResult tap, int insertAt)
{
    currentTap_ComboBox->Delete(insertAt);
    currentTap_ComboBox->Insert(GetCurrentTapDisplayText(tap), insertAt);
    currentTap_ComboBox->SetSelection(insertAt);
}

void MainFrame::OnConnectButtonClick_Internal()
{
    namespace ss = Settings_SRV;
    namespace sdn = SettingDefaultValue_Networks;

    auto tapSelection = currentTap_ComboBox->GetSelection();
    if (tapSelection == wxNOT_FOUND) {
        wxMessageDialog(this, _("You need a virtual network adapter to connect network\nOpen Tools - Manage virtual network adapter to install one")).ShowModal();
        return;
    }

    auto networkSelection = currentNetwork_ComboBox->GetSelection();
    auto& networkRawData = currentNetwork_ComboBox_RawData[networkSelection];
    OnNetworkConnected(networkRawData.get());

    auto& tapRawData = currentTap_ComboBox_RawData[tapSelection];
    if (tapRawData.Available()) {
        tapRawData.Connect();
        UpdateCurrentTapItemDisplayText(tapRawData, tapSelection);

        auto tapSettingsKey = SettingKeys_Networks::network_tap(networkRawData->network.networkName);
        ss::networksConfig->Write(tapSettingsKey, wxString(tapRawData.friendlyName));

        auto recentUsedNetworkSettingsKey = SettingKeys_Networks::default_recentUsedNetwork;
        ss::networksConfig->Write(recentUsedNetworkSettingsKey, wxString(networkRawData->network.networkName));

        auto verboseSettingsKey = SettingKeys_Networks::network_verbose(networkRawData->network.networkName);
        bool verboseExists = ss::networksConfig->HasEntry(verboseSettingsKey);
        if (verboseExists == false) {
            ss::networksConfig->Write(verboseSettingsKey, sdn::verbose);
        }
        auto gameModeSettingsKey = SettingKeys_Networks::network_gameMode(networkRawData->network.networkName);
        auto gameModeExists = ss::networksConfig->HasEntry(gameModeSettingsKey);
        if (gameModeExists == false) {
            ss::networksConfig->Write(gameModeSettingsKey, sdn::gameMode);
        }
        auto autoStartSettingsKey = SettingKeys_Networks::network_autoStart(networkRawData->network.networkName);
        auto autoStartExists = ss::networksConfig->HasEntry(gameModeSettingsKey);
        if (autoStartExists == false) {
            ss::networksConfig->Write(autoStartSettingsKey, sdn::autoStart);
        }
        auto portSettingsKey = SettingKeys_Networks::network_port(networkRawData->network.networkName);
        auto portExists = ss::networksConfig->HasEntry(portSettingsKey);
        if (portExists == false) {
            ss::networksConfig->Write(portSettingsKey, sdn::port);
        }
        auto setMetricSettingsKey = SettingKeys_Networks::network_setMetric(networkRawData->network.networkName);
        auto setMetricExists = ss::networksConfig->HasEntry(setMetricSettingsKey);
        if (setMetricExists == false) {
            ss::networksConfig->Write(setMetricSettingsKey, sdn::setMetric);
        }

        ss::networksConfig->Flush();

        networkRawData->tap = &tapRawData;
        networkRawData->tapSelection = tapSelection;
        std::thread t1(&MainFrame::API_SRV_ConnectToNetwork, this, networkRawData.get());
        t1.detach();
    }
    else {
        wxMessageDialog(this, _("Selected virtual network adapter already connected to another network")).ShowModal();
        OnNetworkDisconnected(networkRawData.get());
    }
}

void MainFrame::OnConnectButtonClick(wxCommandEvent& evt)
{
    OnConnectButtonClick_Internal();
}

void MainFrame::OnDisconnectButtonClick(wxCommandEvent& evt)
{
    // Beware, part of disconnect GUI logic are threaded
    // API_UI_EndConnectToNetwork
    // Disconect button only tell disconnect, but will not handle result

    auto networkSelection = currentNetwork_ComboBox->GetSelection();
    auto& networkRawData = currentNetwork_ComboBox_RawData[networkSelection];

    networkRawData->disconnectButton->Enable(false);
    auto disconnectResult = API_SRV_DisconnectNetwork(networkRawData.get());
    if (disconnectResult.success == false) {
        wxMessageDialog(this, _("Failed to disconnect, tinc.exe may not exist")).ShowModal();
        networkRawData->disconnectButton->Enable(true);
        allowCloseFrame = true;
        return;
    }
}

void MainFrame::OnIpCopyAndRefreshButtonClick(wxCommandEvent& evt)
{
    auto networkSelection = currentNetwork_ComboBox->GetSelection();
    auto& networkRawData = currentNetwork_ComboBox_RawData[networkSelection];

    networkRawData->ipTextCtrl->SelectAll();
    networkRawData->ipTextCtrl->Copy();
    networkRawData->ipTextCtrl->SelectNone();

    auto ipReportThreadW = std::weak_ptr<std::thread>(networkRawData->ipReportThread);
    if (auto ipReportThreadL = ipReportThreadW.lock()) {
        networkRawData->requestIpReportThreadStop = true;
        ipReportThreadL->join();
        networkRawData->requestIpReportThreadStop = false;
        networkRawData->ipReportThread.reset();
    }
    networkRawData->ipReportThread = std::shared_ptr<std::thread>(new std::thread(&PerNetworkData::IpReportThread, networkRawData));
}

void MainFrame::OnNetworkConnected(PerNetworkData* perNetworkData)
{
    allowCloseFrame = false;
    perNetworkData->connectButton->Enable(false);
    perNetworkData->connected = true;

    auto ipReportThreadW = std::weak_ptr<std::thread>(perNetworkData->ipReportThread);
    if (auto ipReportThreadL = ipReportThreadW.lock()) {
        perNetworkData->requestIpReportThreadStop = true;
        ipReportThreadL->join();
        perNetworkData->requestIpReportThreadStop = false;
        perNetworkData->ipReportThread.reset();
    }
    perNetworkData->ipReportThread = std::shared_ptr<std::thread>(new std::thread(&PerNetworkData::IpReportThread, perNetworkData));
}

void MainFrame::OnNetworkDisconnected(PerNetworkData* perNetworkData)
{
    auto ipReportThreadW = std::weak_ptr<std::thread>(perNetworkData->ipReportThread);
    if (auto ipReportThreadL = ipReportThreadW.lock()) {
        perNetworkData->requestIpReportThreadStop = true;
        ipReportThreadL->join();
        perNetworkData->requestIpReportThreadStop = false;
        perNetworkData->ipReportThread.reset();
    }

    perNetworkData->connectButton->Enable(true);
    perNetworkData->connected = false;
    allowCloseFrame = true;
}

void MainFrame::OnClose(wxCloseEvent& event)
{
    if (allowCloseFrame == false) {
        wxMessageDialog(this, _("Stop ALL running tasks before close program")).ShowModal();
        return;
    }

    event.Skip();
}

void PerNetworkData::IpReportThread()
{
    const long long sleepStepMs = 200;
    const long long secondInMs = sleepStepMs * 5;
    const long long totalSleepTimeSec = secondInMs * 20;
    long long sleeped = 0;

    while (connected) {
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepStepMs));
        sleeped = sleeped + sleepStepMs;
        if (requestIpReportThreadStop) {
            return;
        }
        if (connected == false) {
            return;
        }
        if (sleeped > totalSleepTimeSec) {
            return;
        }
        if (sleeped % secondInMs != 0) {
            continue;
        }

        std::vector<std::string> result;
        WindowsAPI_SRV::GetAdaptersInfo(tap->windows_LUID, &result);

        // result.size() == 1: "192.168.1.1"
        // result.size() == 3: "192.168.1.1, 192.168.1.2, 192.168.1.3"
        auto resultSize = result.size();
        std::stringstream wss;
        if (resultSize > 0) {
            wss << result[0];
            if (resultSize > 1) {
                const std::string delimiter = ", ";
                for (size_t i = 1; i < result.size(); i++)
                {
                    wss << delimiter << result[i];
                }
            }
        }
        auto assignedIPs = wss.str();

        parentFrame->CallAfter([this, assignedIPs] {
            auto ipTextCtrlW = std::weak_ptr<wxTextCtrl>(ipTextCtrl);
            if (auto ipTextCtrlL = ipTextCtrlW.lock()) {
                ipTextCtrlL->SetValue(assignedIPs);
                ipTextCtrlL->SetInsertionPointEnd();
            }
            });
    }
}

wxFrame* PerNetworkData::parentFrame;
