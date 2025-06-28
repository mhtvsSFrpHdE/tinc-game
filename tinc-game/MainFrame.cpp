#include "MainFrame.h"
#include <wx/spinctrl.h>
#include "OptimizeMtuFrame.h"
#include "SettingsFrame.h"
#include <wx/dialog.h>
#include "ManageTapFrame.h"
#include "Resource_SRV.h"
#include <sstream>
#include "String_SRV.h"
#include "HelpFrame.h"
#include "Settings_SRV.h"
#include "Networks_SRV.h"
#include "TapDevice_SRV.h"

void MainFrame::API_UI_SetDisconnectButtonEnable(bool enable, wxButton* disconnectButton)
{
    disconnectButton->Enable(enable);
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
        liveLogSizer->Replace(recentActiveLiveLog.get(), rawData.liveLog.get());
        rawData.liveLog->Show();
        recentActiveLiveLog = rawData.liveLog;

        liveLogSizer->Layout();
    }

    {
        recentActiveConnectButton->Hide();
        networkControlSizer->Replace(recentActiveConnectButton.get(), rawData.connectButton.get());
        rawData.connectButton->Show();
        recentActiveConnectButton = rawData.connectButton;

        recentActiveDisconnectButton->Hide();
        networkControlSizer->Replace(recentActiveDisconnectButton.get(), rawData.disconnectButton.get());
        rawData.disconnectButton->Show();
        recentActiveDisconnectButton = rawData.disconnectButton;

        networkControlSizer->Layout();
    }

    {
        for (size_t i = 0; i < currentTap_ComboBox_RawData.size(); i++)
        {
            auto& adapter = currentTap_ComboBox_RawData[i];

            if (rawData.tap) {
                if (adapter.friendlyName == rawData.tap->friendlyName) {
                    currentTap_ComboBox->SetSelection(i);
                    return;
                }
            }

            if (adapter.Available() && adapter.friendlyName == rawData.network.recentUsedTapName) {
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

    auto tapSelection = currentTap_ComboBox->GetSelection();
    if (tapSelection == wxNOT_FOUND) {
        return;
    }

    auto networkSelection = currentNetwork_ComboBox->GetSelection();
    auto& networkRawData = currentNetwork_ComboBox_RawData[networkSelection];
    networkRawData.connectButton->Enable(false);
    networkRawData.connected = true;

    auto& tapRawData = currentTap_ComboBox_RawData[tapSelection];
    if (tapRawData.Available()) {
        tapRawData.Connect();
        UpdateCurrentTapItemDisplayText(tapRawData, tapSelection);

        auto tapSettingsKey = SettingKeys_Networks::network_tap(networkRawData.network.networkName);
        ss::networksConfig->Write(tapSettingsKey, wxString(tapRawData.friendlyName));

        auto recentUsedNetworkSettingsKey = SettingKeys_Networks::default_recentUsedNetwork;
        ss::networksConfig->Write(recentUsedNetworkSettingsKey, wxString(networkRawData.network.networkName));

        auto verboseSettingsKey = SettingKeys_Networks::network_verbose(networkRawData.network.networkName);
        bool verboseExists = ss::networksConfig->HasEntry(verboseSettingsKey);
        if (verboseExists == false) {
            ss::networksConfig->Write(verboseSettingsKey, true);
        }
        auto gameModeSettingsKey = SettingKeys_Networks::network_gameMode(networkRawData.network.networkName);
        auto gameModeExists = ss::networksConfig->HasEntry(gameModeSettingsKey);
        if (gameModeExists == false) {
            ss::networksConfig->Write(gameModeSettingsKey, false);
        }
        auto autoStartSettingsKey = SettingKeys_Networks::network_autoStart(networkRawData.network.networkName);
        auto autoStartExists = ss::networksConfig->HasEntry(gameModeSettingsKey);
        if (autoStartExists == false) {
            ss::networksConfig->Write(autoStartSettingsKey, false);
        }
        auto portSettingsKey = SettingKeys_Networks::network_port(networkRawData.network.networkName);
        auto portExists = ss::networksConfig->HasEntry(portSettingsKey);
        if (portExists == false) {
            ss::networksConfig->Write(portSettingsKey, 0);
        }
        auto setMetricSettingsKey = SettingKeys_Networks::network_setMetric(networkRawData.network.networkName);
        auto setMetricExists = ss::networksConfig->HasEntry(setMetricSettingsKey);
        if (setMetricExists == false) {
            ss::networksConfig->Write(setMetricSettingsKey, true);
        }

        ss::networksConfig->Flush();

        networkRawData.tap = &tapRawData;
        networkRawData.tapSelection = tapSelection;
        std::thread t1(&MainFrame::API_SRV_ConnectToNetwork, this, &networkRawData);
        t1.detach();
    }
    else {
        wxMessageDialog(this, _("Selected virtual network adapter already connected to another network")).ShowModal();
        networkRawData.connectButton->Enable(true);
        networkRawData.connected = false;
    }
}

void MainFrame::OnConnectButtonClick(wxCommandEvent& evt)
{
    OnConnectButtonClick_Internal();
}

void MainFrame::OnDisconnectButtonClick(wxCommandEvent& evt)
{
    auto networkSelection = currentNetwork_ComboBox->GetSelection();
    auto& networkRawData = currentNetwork_ComboBox_RawData[networkSelection];

    networkRawData.disconnectButton->Enable(false);
    auto disconnectResult = API_SRV_DisconnectNetwork(&networkRawData);
    if (disconnectResult.success == false) {
        wxMessageDialog(this, _("Failed to disconnect, tinc.exe may not exist")).ShowModal();
        networkRawData.disconnectButton->Enable(true);
        return;
    }
    networkRawData.connectButton->Enable(true);
    networkRawData.connected = false;
}

void MainFrame::OnOptimizeMtuButton(wxCommandEvent& evt)
{
    optimizeMtuButton->Enable(false);
    openedFrameCount++;

    std::function<void()> redirectCallback = std::bind(&MainFrame::OnOptimizeMtuFrameCloseCallback, this);
    OptimizeMtuFrame* optimizeMtuframe = new OptimizeMtuFrame(this, redirectCallback);
    optimizeMtuframe->Center();
    optimizeMtuframe->Show();
}

void MainFrame::OnOptimizeMtuFrameCloseCallback()
{
    openedFrameCount--;
    optimizeMtuButton->Enable(true);
}

void MainFrame::OnManageTapButton(wxCommandEvent& evt)
{
    bool allAllowEdit = AllowMakeChange();
    if (allAllowEdit == false) {
        return;
    }

    std::function<void()> redirectCallback = std::bind(&MainFrame::OnManageTapButtonCloseCallback, this);
    ManageTapFrame* manageTapDeviceFrame = new ManageTapFrame(this, redirectCallback);
    manageTapDeviceFrame->Center();
    manageTapDeviceFrame->Show();
}

void MainFrame::OnManageTapButtonCloseCallback()
{
    OnMenuNetworksReload_Internal();
}

void MainFrame::OnIntegrityCheckButton(wxCommandEvent& evt)
{
    integrityCheckButton->Enable(false);

    auto checkResult = Resource_SRV::IntegrityCheck();

    std::function<void()> redirectCallback = std::bind(&MainFrame::OnIntegrityCheckFrameCloseCallback, this);
    HelpFrame* optimizeMtuFrame_HelpFrame = new HelpFrame(this, _("Troubleshoot"), redirectCallback);
    optimizeMtuFrame_HelpFrame->SetHelpText(checkResult);
    optimizeMtuFrame_HelpFrame->Center();
    optimizeMtuFrame_HelpFrame->Show();
}

void MainFrame::OnIntegrityCheckFrameCloseCallback()
{
    integrityCheckButton->Enable(true);
}

void OnSettingsButton_OpenSettingsFrame(MainFrame* mainFrame)
{
    SettingsFrame* settingsframe = new SettingsFrame(mainFrame);
    settingsframe->Center();
    settingsframe->Show();
}

void OnSettingsButton_OtherWindowExists(MainFrame* mainFrame)
{
    wxString buttonHint = _("Close all windows before enter setting interface.");
    wxString title = _("hint");
    wxMessageDialog* dial = new wxMessageDialog(NULL,
        buttonHint, title, wxOK);
    dial->ShowModal();
}

void MainFrame::OnSettingsButton(wxCommandEvent& evt)
{
    if (openedFrameCount == 0)
    {
        OnSettingsButton_OpenSettingsFrame(this);
    }
    else {
        OnSettingsButton_OtherWindowExists(this);
    }
}
