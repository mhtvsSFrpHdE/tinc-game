#include "MainFrame.h"
#include <wx/spinctrl.h>
#include "OptimizeMtuFrame.h"
#include "SettingsFrame.h"
#include <wx/dialog.h>
#include "Layout_SRV.h"
#include "ManageTapFrame.h"
#include <wx/filename.h>
#include <sstream>
#include "String_SRV.h"
#include "HelpFrame.h"
#include "Settings_SRV.h"
#include "Networks_SRV.h"
#include "TapDevice_SRV.h"

void MainFrame::API_UI_ReportStatus(std::wstring status, tincTextCtrl* liveLog)
{
    liveLog->tincAppendText(status);
}

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, _("Tinc Game Mode")) {
    Init_CreateControls();
    Init_Layout();
}

void MainFrame::Init_CreateControls()
{
    rootPanel = new wxPanel(this);

    connectButtonPlaceholder = new wxButton(rootPanel, wxID_ANY, _("Connect"));
    connectButtonPlaceholder->Enable(false);
    disconnectButtonPlaceholder = new wxButton(rootPanel, wxID_ANY, _("Disconnect"));
    disconnectButtonPlaceholder->Enable(false);
    liveLogPlaceholder = new tincTextCtrl(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);
    liveLogPlaceholder->Enable(false);

    currentNetwork_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Network name"));
    currentNetwork_ComboBox = new wxComboBox(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
    {
        namespace ns = Networks_SRV;
        auto getNetworks = ns::GetNetworks();
        if (getNetworks.success) {
            int mapIndex = 0;
            for (int networkIndex = 0; networkIndex < getNetworks.returnBody.size(); networkIndex++)
            {
                PerNetworkData perNetworkData;
                perNetworkData.network = getNetworks.returnBody[networkIndex];
                perNetworkData.liveLog = new tincTextCtrl(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);
                perNetworkData.liveLog->tincSetMaxLines(100);
                perNetworkData.liveLog->Hide();
                perNetworkData.connectButton = new wxButton(rootPanel, wxID_ANY, _("Connect"));
                perNetworkData.connectButton->Bind(wxEVT_BUTTON, &MainFrame::OnConnectButtonClick, this);
                perNetworkData.connectButton->Hide();
                perNetworkData.disconnectButton = new wxButton(rootPanel, wxID_ANY, _("Disconnect"));
                perNetworkData.disconnectButton->Bind(wxEVT_BUTTON, &MainFrame::OnDisconnectButtonClick, this);
                perNetworkData.disconnectButton->Hide();

                currentNetwork_ComboBox_RawData.insert({ mapIndex, perNetworkData });
                currentNetwork_ComboBox->Append(perNetworkData.network.networkName);
                mapIndex = mapIndex + 1;
            }
        }
    }
    currentNetwork_ComboBox->Bind(wxEVT_COMBOBOX, &MainFrame::OnCurrentNetworkChange, this);

    currentTap_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Connect with"));
    currentTap_ComboBox = new wxComboBox(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
    {
        auto getNetworkAdapterList = TapDevice_SRV::API_SRV_GetNetworkAdapterList();
        if (getNetworkAdapterList.success) {
            currentTap_ComboBox->Clear();
            currentTap_ComboBox_RawData.clear();

            int mapIndex = 0;
            for (int adapterIndex = 0; adapterIndex < getNetworkAdapterList.returnBody.size(); adapterIndex++)
            {
                auto adapter = getNetworkAdapterList.returnBody[adapterIndex];
                bool isNotLoopback = adapter.IsLoopback() == false;
                bool isTap = adapter.IsTap();
                if (isNotLoopback && isTap) {
                    currentTap_ComboBox_RawData.insert({ mapIndex, adapter });
                    currentTap_ComboBox->Append(adapter.friendlyName + " | " + (adapter.Available() ? _("Available") : _("Connected")));

                    mapIndex = mapIndex + 1;
                }
            }
        }
    }
    currentTap_ComboBox->Bind(wxEVT_COMBOBOX, &MainFrame::OnCurrentTapChange, this);

    optimizeMtuButton = new wxButton(rootPanel, wxID_ANY, _("Optimize MTU"));
    optimizeMtuButton->Bind(wxEVT_BUTTON, &MainFrame::OnOptimizeMtuButton, this);
    manageTapDeviceButton = new wxButton(rootPanel, wxID_ANY, _("Manage virtual network adapter"));
    manageTapDeviceButton->Bind(wxEVT_BUTTON, &MainFrame::OnManageTapButton, this);
    integrityCheckButton = new wxButton(rootPanel, wxID_ANY, _("Troubleshoot"));
    integrityCheckButton->Bind(wxEVT_BUTTON, &MainFrame::OnIntegrityCheckButton, this);
    settingsButton = new wxButton(rootPanel, wxID_ANY, _("Settings"));
    settingsButton->Bind(wxEVT_BUTTON, &MainFrame::OnSettingsButton, this);
}

void MainFrame::Init_Layout()
{
    namespace ls = Layout_SRV;

    wxSize minSize(640, 480);
    this->SetSizeHints(minSize);

    wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
    rootPanel->SetSizer(rootSizer);
    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    {
        wxBoxSizer* currentNetworkRootSizer = new wxBoxSizer(wxHORIZONTAL);
        rootSizer->Add(currentNetworkRootSizer, 0, wxEXPAND);

        wxBoxSizer* networkSizer = new wxBoxSizer(wxVERTICAL);
        currentNetworkRootSizer->Add(networkSizer, 1);
        networkSizer->Add(currentNetwork_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
        ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, networkSizer);
        networkSizer->Add(currentNetwork_ComboBox, 1, wxEXPAND | wxLEFT, ls::SpaceToFrameBorder);
        ls::AddFixedSpacer(wxLEFT, ls::SpaceBetweenControl, currentNetworkRootSizer);

        wxBoxSizer* tapSizer = new wxBoxSizer(wxVERTICAL);
        currentNetworkRootSizer->Add(tapSizer, 1);
        tapSizer->Add(currentTap_StaticText);
        ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, tapSizer);
        tapSizer->Add(currentTap_ComboBox, 1, wxEXPAND | wxRIGHT, ls::SpaceToFrameBorder);

        ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
    }


    {
        networkControlSizer = new wxBoxSizer(wxHORIZONTAL);
        rootSizer->Add(networkControlSizer);
        networkControlSizer->Add(connectButtonPlaceholder, 1, wxLEFT, ls::SpaceToFrameBorder);
        networkControlSizer->Add(disconnectButtonPlaceholder, 1, wxLEFT, ls::SpaceBetweenControl);

        ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
    }

    {
        liveLogSizer = new wxBoxSizer(wxHORIZONTAL);
        rootSizer->Add(liveLogSizer, 1, wxEXPAND);
        liveLogSizer->Add(0, 0, 0, wxLEFT, ls::SpaceToFrameBorder);
        liveLogSizer->Add(liveLogPlaceholder, 1, wxEXPAND);
        liveLogSizer->Add(0, 0, 0, wxRIGHT, ls::SpaceToFrameBorder);

        ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
    }

    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    {
        wxBoxSizer* buttonRootSizer = new wxBoxSizer(wxHORIZONTAL);
        rootSizer->Add(buttonRootSizer, 0, wxEXPAND);
        wxBoxSizer* buttonLeftSizer = new wxBoxSizer(wxVERTICAL);
        buttonRootSizer->Add(buttonLeftSizer, 1, wxEXPAND);
        wxBoxSizer* buttonRightSizer = new wxBoxSizer(wxVERTICAL);
        buttonRootSizer->Add(buttonRightSizer, 2, wxEXPAND);
        wxPanel* rightSizerPanel = new wxPanel(rootPanel);
        buttonRootSizer->Add(rightSizerPanel, 2, wxEXPAND);

        wxBoxSizer* optimizeMtuSizer = new wxBoxSizer(wxHORIZONTAL);
        optimizeMtuSizer->Add(0, 0, 0, wxLEFT, ls::SpaceToFrameBorder);
        optimizeMtuSizer->Add(optimizeMtuButton, 1, wxEXPAND);
        optimizeMtuSizer->Add(0, 0, 0, wxRIGHT, ls::SpaceToFrameBorder);
        buttonLeftSizer->Add(optimizeMtuSizer, 1, wxEXPAND);
        ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, buttonLeftSizer);

        wxBoxSizer* manageTapDeviceSizer = new wxBoxSizer(wxHORIZONTAL);
        manageTapDeviceSizer->Add(0, 0, 0, wxLEFT, ls::SpaceToFrameBorder);
        manageTapDeviceSizer->Add(manageTapDeviceButton, 1, wxEXPAND);
        manageTapDeviceSizer->Add(0, 0, 0, wxRIGHT, ls::SpaceToFrameBorder);
        buttonLeftSizer->Add(manageTapDeviceSizer, 1, wxEXPAND);
        ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, buttonLeftSizer);

        wxBoxSizer* integrityCheckSizer = new wxBoxSizer(wxHORIZONTAL);
        integrityCheckSizer->Add(0, 0, 0, wxLEFT, ls::SpaceToFrameBorder);
        integrityCheckSizer->Add(integrityCheckButton, 1, wxEXPAND);
        integrityCheckSizer->Add(0, 0, 0, wxRIGHT, ls::SpaceToFrameBorder);
        buttonLeftSizer->Add(integrityCheckSizer, 1, wxEXPAND);
        ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, buttonLeftSizer);

        wxBoxSizer* settingsSizer = new wxBoxSizer(wxHORIZONTAL);
        settingsSizer->Add(0, 0, 0, wxLEFT, ls::SpaceToFrameBorder);
        settingsSizer->Add(settingsButton, 1, wxEXPAND);
        settingsSizer->Add(0, 0, 0, wxRIGHT, ls::SpaceToFrameBorder);
        buttonLeftSizer->Add(settingsSizer, 1, wxEXPAND);
        ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, buttonLeftSizer);
    }

    this->SetSize(minSize);
}

void MainFrame::OnCurrentNetworkChange(wxCommandEvent& evt)
{
    auto selection = currentNetwork_ComboBox->GetSelection();
    auto rawData = currentNetwork_ComboBox_RawData[selection];

    {
        liveLogPlaceholder->Hide();
        liveLogSizer->Replace(liveLogPlaceholder, rawData.liveLog);
        rawData.liveLog->Show();
        liveLogPlaceholder = rawData.liveLog;

        liveLogSizer->Layout();
    }

    {
        connectButtonPlaceholder->Hide();
        networkControlSizer->Replace(connectButtonPlaceholder, rawData.connectButton);
        rawData.connectButton->Show();
        connectButtonPlaceholder = rawData.connectButton;

        disconnectButtonPlaceholder->Hide();
        networkControlSizer->Replace(disconnectButtonPlaceholder, rawData.disconnectButton);
        rawData.disconnectButton->Show();
        disconnectButtonPlaceholder = rawData.disconnectButton;

        networkControlSizer->Layout();
    }

    {
        for (int i = 0; i < currentTap_ComboBox_RawData.size(); i++)
        {
            auto adapter = currentTap_ComboBox_RawData[i];

            if (adapter.friendlyName == rawData.network.recentUsedTapName) {
                currentTap_ComboBox->SetSelection(i);
                currentTap_ComboBox->SendSelectionChangedEvent(wxEVT_COMBOBOX);
                return;
            }
        }
        for (int i = 0; i < currentTap_ComboBox_RawData.size(); i++) {
            auto adapter = currentTap_ComboBox_RawData[i];

            if (adapter.Available()) {
                currentTap_ComboBox->SetSelection(i);
                currentTap_ComboBox->SendSelectionChangedEvent(wxEVT_COMBOBOX);
                return;
            }
        }
    }
}

void MainFrame::OnCurrentTapChange(wxCommandEvent& evt)
{
    // TODO: PerNetworkData tap set to this ?
}

void MainFrame::OnConnectButtonClick(wxCommandEvent& evt)
{
    auto networkSelection = currentNetwork_ComboBox->GetSelection();
    auto networkRawData = currentNetwork_ComboBox_RawData[networkSelection];
    networkRawData.connectButton->Enable(false);

    auto tapSelection = currentTap_ComboBox->GetSelection();
    auto tapRawData = currentTap_ComboBox_RawData[tapSelection];

    networkRawData.tap = tapRawData;

    // TODO: Settings save recent used tap name

    std::thread t1(&MainFrame::API_SRV_ConnectToNetwork, this, networkRawData);
    t1.detach();
}

void MainFrame::OnDisconnectButtonClick(wxCommandEvent& evt)
{
    //auto tap = currentTap_ComboBox_RawData[currentTap_ComboBox->GetSelection()];
    //tap.disconnectButton->Enable(false);
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
    ManageTapFrame* manageTapDeviceFrame = new ManageTapFrame(this);
    manageTapDeviceFrame->Center();
    manageTapDeviceFrame->Show();
}

int OnIntegrityCheckButton_FailedCount = 0;
void OnIntegrityCheckButton_DirExist(std::wostringstream& helpText, wxFileName& file) {
    namespace ss = String_SRV;

    bool exists = file.DirExists();
    helpText << file.GetFullPath() << ss::ellipses
        << (exists ? _("OK") : _("Fail"))
        << ss::newLine;
    if (exists == false) {
        OnIntegrityCheckButton_FailedCount++;
    }
}

void OnIntegrityCheckButton_FileExist(std::wostringstream& helpText, wxFileName& file) {
    namespace ss = String_SRV;

    bool exists = file.Exists();
    helpText << file.GetFullPath() << ss::ellipses
        << (exists ? _("OK") : _("Fail"))
        << ss::newLine;
    if (exists == false) {
        OnIntegrityCheckButton_FailedCount++;
    }
}

void MainFrame::OnIntegrityCheckButton(wxCommandEvent& evt)
{
    integrityCheckButton->Enable(false);
    OnIntegrityCheckButton_FailedCount = 0;

    namespace ss = String_SRV;

    std::wostringstream helpText;
    wxFileName file;

    helpText << _("Program integrity report")
        << ss::newLine << ss::newLine;

    {
        helpText << _("Virtual network adapter driver files:") << ss::newLine;

        file.AppendDir("bin");
        file.AppendDir("tinc");
        file.AppendDir("tap-win64");

        file.SetName(L"tapinstall.exe");
        OnIntegrityCheckButton_FileExist(helpText, file);
        file.SetName(L"OemWin2k.inf");
        OnIntegrityCheckButton_FileExist(helpText, file);
        file.SetName(L"tap0901.cat");
        OnIntegrityCheckButton_FileExist(helpText, file);
        file.SetName(L"tap0901.sys");
        OnIntegrityCheckButton_FileExist(helpText, file);

        helpText << ss::newLine;
        file.Clear();
    }

    {
        helpText << _("Command line I/O files:") << ss::newLine;

        file.SetName(L"getTapHwid.bat");
        OnIntegrityCheckButton_FileExist(helpText, file);
        file.SetName(L"installTap.bat");
        OnIntegrityCheckButton_FileExist(helpText, file);
        file.SetName(L"uninstallTap.bat");
        OnIntegrityCheckButton_FileExist(helpText, file);
        file.SetName(L"netsh437.bat");
        OnIntegrityCheckButton_FileExist(helpText, file);
        file.SetName(L"netsh437_start.bat");
        OnIntegrityCheckButton_FileExist(helpText, file);
        file.SetName(L"ping437.bat");
        OnIntegrityCheckButton_FileExist(helpText, file);
        file.SetName(L"ping437_start.bat");
        OnIntegrityCheckButton_FileExist(helpText, file);

        helpText << ss::newLine;
        file.Clear();
    }

    {
        helpText << _("Translation files:") << ss::newLine;

        file.AppendDir(L"zh");
        OnIntegrityCheckButton_DirExist(helpText, file);

        helpText << ss::newLine;
        file.Clear();
    }

    if (OnIntegrityCheckButton_FailedCount >= 4) {
        helpText << _("Failed items more than 4, don't double click tinc-game in zip/7z/rar files directly you foo")
            << ss::newLine << ss::newLine;
    }

    std::function<void()> redirectCallback = std::bind(&MainFrame::OnIntegrityCheckFrameCloseCallback, this);
    HelpFrame* optimizeMtuFrame_HelpFrame = new HelpFrame(this, _("Troubleshoot"), redirectCallback);
    optimizeMtuFrame_HelpFrame->SetHelpText(helpText.str());
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
