#include "MainFrame.h"
#include "Settings_SRV.h"
#include "Layout_SRV.h"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, _("Tinc Game Mode")) {
    Init_CreateControls();
    Init_Layout();
    Init_PostLayout();
}

void MainFrame::Init_CreateControls()
{
    namespace ss = Settings_SRV;

    menuBar = new wxMenuBar();

    networksMenu = new wxMenu();
    networksMenu->Append(wxIdMenuNetworksEdit, _T("&Edit"));
    Bind(wxEVT_MENU, &MainFrame::OnMenuNetworksEdit, this, wxIdMenuNetworksEdit);
    networksMenu->Append(wxIdMenuNetworksJoin, _T("&Join network"));
    Bind(wxEVT_MENU, &MainFrame::OnMenuNetworksJoin, this, wxIdMenuNetworksJoin);
    networksMenu->Append(wxIdMenuNetworksRename, _T("&Rename"));
    Bind(wxEVT_MENU, &MainFrame::OnMenuNetworksRename, this, wxIdMenuNetworksRename);

    networksMenu->AppendSeparator();

    networksAdvancedMenu = new wxMenu();
    networksAdvancedMenu->Append(wxIdMenuNetworksAdvancedDelete, _T("&Delete"));
    Bind(wxEVT_MENU, &MainFrame::OnMenuNetworksAdvancedDelete, this, wxIdMenuNetworksAdvancedDelete);
    networksMenu->Append(wxID_ANY, _T("&Advanced"), networksAdvancedMenu);

    menuBar->Append(networksMenu, _T("&Networks"));
    SetMenuBar(menuBar);

    rootPanel = new wxPanel(this);
    connectButtonPlaceholder = new wxButton(rootPanel, wxID_ANY, _("Connect"));
    connectButtonPlaceholder->Enable(false);
    disconnectButtonPlaceholder = new wxButton(rootPanel, wxID_ANY, _("Disconnect"));
    disconnectButtonPlaceholder->Enable(false);
    liveLogPlaceholder = new tincTextCtrl(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);
    liveLogPlaceholder->Enable(false);

    currentTap_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Connect with"));
    currentTap_ComboBox = new wxComboBox(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
    {
        auto getNetworkAdapterList = TapDevice_SRV::API_SRV_GetNetworkAdapterList();
        if (getNetworkAdapterList.success) {
            currentTap_ComboBox->Clear();
            currentTap_ComboBox_RawData.clear();

            int mapIndex = 0;
            for (size_t adapterIndex = 0; adapterIndex < getNetworkAdapterList.returnBody.size(); adapterIndex++)
            {
                auto adapter = getNetworkAdapterList.returnBody[adapterIndex];
                bool isNotLoopback = adapter.IsLoopback() == false;
                bool isTap = adapter.IsTap();
                if (isNotLoopback && isTap) {
                    currentTap_ComboBox_RawData.insert({ mapIndex, adapter });
                    currentTap_ComboBox->Append(GetCurrentTapDisplayText(adapter));

                    mapIndex = mapIndex + 1;
                }
            }
        }
    }

    currentNetwork_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Network name"));
    currentNetwork_ComboBox = new wxComboBox(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
    currentNetwork_ComboBox->Bind(wxEVT_COMBOBOX, &MainFrame::OnCurrentNetworkChange, this);
    {
        namespace ns = Networks_SRV;
        auto getNetworks = ns::GetNetworks();
        if (getNetworks.success) {
            auto recentUsedNetwork = ss::networksConfig->Read(SettingKeys_Networks::default_recentUsedNetwork);

            int mapIndex = 0;
            for (size_t networkIndex = 0; networkIndex < getNetworks.returnBody.size(); networkIndex++)
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
                perNetworkData.disconnectButton->Enable(false);
                perNetworkData.disconnectButton->Hide();

                if (perNetworkData.network.networkName == recentUsedNetwork) {
                    recentUsedNetworkSelection = mapIndex;
                }

                currentNetwork_ComboBox_RawData.insert({ mapIndex, perNetworkData });
                currentNetwork_ComboBox->Append(perNetworkData.network.networkName);

                auto autoStartSettingsKey = SettingKeys_Networks::network_autoStart(perNetworkData.network.networkName);
                bool autoStart = ss::networksConfig->ReadBool(autoStartSettingsKey, false);
                if (autoStart) {
                    autoStartNetworkRawDataIndex_pending.push_back(mapIndex);
                }

                mapIndex = mapIndex + 1;
            }
            if (recentUsedNetworkSelection == wxNOT_FOUND) {
                recentUsedNetworkSelection = 0;
            }
        }
    }

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

    this->Fit();
}

void MainFrame::Init_PostLayout()
{
    std::thread t1(&MainFrame::API_SRV_PostLayout, this);
    t1.detach();
}
