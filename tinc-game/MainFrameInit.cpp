#include "MainFrame.h"
#include "Settings_SRV.h"
#include "Layout_SRV.h"
#include "Resource_SRV.h"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, _("Tinc Game Mode")) {
    Enable(false);
    Init_CreateControls();
    Init_Layout();
    Init_PostLayout();
}

std::shared_ptr<wxButton> MainFrame::GetInitPhaseDummyConnectButton()
{
    auto button = std::shared_ptr<wxButton>(new wxButton(rootPanel, wxID_ANY, _("Connect")), Resource_SRV::wxWidgets::wxButtonDeleter);
    button->Enable(false);
    return button;
}

std::shared_ptr<wxButton> MainFrame::GetInitPhaseDummyDisconnectButton()
{
    auto button = std::shared_ptr<wxButton>(new wxButton(rootPanel, wxID_ANY, _("Disconnect")), Resource_SRV::wxWidgets::wxButtonDeleter);
    button->Enable(false);
    return button;
}

std::shared_ptr<tincTextCtrl> MainFrame::GetInitPhaseDummyLiveLog()
{
    auto textCtrl = std::shared_ptr<tincTextCtrl>(new tincTextCtrl(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE), Resource_SRV::wxWidgets::wxTextCtrlDeleter);
    textCtrl->Enable(false);
    return textCtrl;
}

std::shared_ptr<wxTextCtrl> MainFrame::GetInitPhaseDummyIpTextCtrl()
{
    auto textCtrl = std::shared_ptr<wxTextCtrl>(new wxTextCtrl(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY), Resource_SRV::wxWidgets::wxTextCtrlDeleter);
    textCtrl->SetBackgroundColour(*wxWHITE);
    return textCtrl;
}

std::shared_ptr<wxButton> MainFrame::GetInitPhaseDummyIpCopyAndRefreshButton()
{
    auto button = std::shared_ptr<wxButton>(new wxButton(rootPanel, wxID_ANY, _("Copy")), Resource_SRV::wxWidgets::wxButtonDeleter);
    button->Enable(false);
    return button;
}

void MainFrame::ReloadCurrentNetwork()
{
    namespace ss = Settings_SRV;
    namespace ns = Networks_SRV;
    namespace rsx = Resource_SRV::wxWidgets;

    auto getNetworks = ns::GetNetworks();
    if (getNetworks.success) {
        currentNetwork_ComboBox->Clear();
        currentNetwork_ComboBox_RawData.clear();

        auto recentUsedNetwork = ss::networksConfig->Read(SettingKeys_Networks::default_recentUsedNetwork);

        int mapIndex = 0;
        for (size_t networkIndex = 0; networkIndex < getNetworks.returnBody.size(); networkIndex++)
        {
            std::shared_ptr<PerNetworkData> perNetworkData(new PerNetworkData());
            perNetworkData->network = getNetworks.returnBody[networkIndex];
            perNetworkData->liveLog = std::shared_ptr<tincTextCtrl>(new tincTextCtrl(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE));
            perNetworkData->liveLog->tincSetMaxLines(100);
            perNetworkData->liveLog->Hide();
            perNetworkData->connectButton = std::shared_ptr<wxButton>(new wxButton(rootPanel, wxID_ANY, _("Connect")), rsx::wxButtonDeleter);
            perNetworkData->connectButton->Bind(wxEVT_BUTTON, &MainFrame::OnConnectButtonClick, this);
            perNetworkData->connectButton->Hide();
            perNetworkData->disconnectButton = std::shared_ptr<wxButton>(new wxButton(rootPanel, wxID_ANY, _("Disconnect")), rsx::wxButtonDeleter);
            perNetworkData->disconnectButton->Bind(wxEVT_BUTTON, &MainFrame::OnDisconnectButtonClick, this);
            perNetworkData->disconnectButton->Enable(false);
            perNetworkData->disconnectButton->Hide();
            perNetworkData->ipTextCtrl = std::shared_ptr<wxTextCtrl>(new wxTextCtrl(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY), rsx::wxTextCtrlDeleter);
            perNetworkData->ipTextCtrl->Hide();
            perNetworkData->ipCopyAndRefreshButton = std::shared_ptr<wxButton>(new wxButton(rootPanel, wxID_ANY, _("Copy")), rsx::wxButtonDeleter);
            perNetworkData->ipCopyAndRefreshButton->Enable(false);
            perNetworkData->ipCopyAndRefreshButton->Hide();

            if (perNetworkData->network.networkName == recentUsedNetwork) {
                recentUsedNetworkSelection = mapIndex;
            }

            currentNetwork_ComboBox_RawData.insert({ mapIndex, perNetworkData });
            currentNetwork_ComboBox->Append(perNetworkData->network.networkName);

            auto autoStartSettingsKey = SettingKeys_Networks::network_autoStart(perNetworkData->network.networkName);
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

void MainFrame::ReloadCurrentTap()
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

void MainFrame::Init_CreateControls()
{
    namespace ss = Settings_SRV;

    menuBar = new wxMenuBar();

    {
        networksMenu = new wxMenu();
        networksMenu->Append(wxIdMenuNetworksEdit, _T("&Edit"));
        Bind(wxEVT_MENU, &MainFrame::OnMenuNetworksEdit, this, wxIdMenuNetworksEdit);
        networksMenu->Append(wxIdMenuNetworksRename, _T("&Rename"));
        Bind(wxEVT_MENU, &MainFrame::OnMenuNetworksRename, this, wxIdMenuNetworksRename);
        networksMenu->Append(wxIdMenuNetworksJoin, _T("&Join network"));
        Bind(wxEVT_MENU, &MainFrame::OnMenuNetworksJoin, this, wxIdMenuNetworksJoin);
        networksMenu->Append(wxIdMenuNetworksImportAndExport, _T("&Import and Export"));
        Bind(wxEVT_MENU, &MainFrame::OnMenuNetworksImportAndExport, this, wxIdMenuNetworksImportAndExport);
        networksMenu->Append(wxIdMenuNetworksReload, _T("&Reload"));
        Bind(wxEVT_MENU, &MainFrame::OnMenuNetworksReload, this, wxIdMenuNetworksReload);

        networksMenu->AppendSeparator();

        networksAdvancedMenu = new wxMenu();
        networksAdvancedMenu->Append(wxIdMenuNetworksAdvancedDelete, _T("&Delete"));
        Bind(wxEVT_MENU, &MainFrame::OnMenuNetworksAdvancedDelete, this, wxIdMenuNetworksAdvancedDelete);
        networksMenu->Append(wxID_ANY, _T("&Advanced"), networksAdvancedMenu);

        menuBar->Append(networksMenu, _T("&Networks"));
    }

    {
        toolsMenu = new wxMenu();
        toolsMenu->Append(wxIdMenuToolsManageTap, _T("&Manage virtual network adapter"));
        Bind(wxEVT_MENU, &MainFrame::OnMenuToolsManageTap, this, wxIdMenuToolsManageTap);
        toolsMenu->Append(wxIdMenuToolsSettings, _T("&Settings"));
        Bind(wxEVT_MENU, &MainFrame::OnMenuToolsSettings, this, wxIdMenuToolsSettings);

        toolsMenu->AppendSeparator();

        toolsAdvancedMenu = new wxMenu();
        toolsAdvancedMenu->Append(wxIdMenuToolsAdvancedOptimizeMtu, _T("&Optimize MTU"));
        Bind(wxEVT_MENU, &MainFrame::OnMenuToolsAdvancedOptimizeMtu, this, wxIdMenuToolsAdvancedOptimizeMtu);
        toolsAdvancedMenu->Append(wxIdMenuToolsAdvancedTroubleshoot, _T("&Troubleshoot"));
        Bind(wxEVT_MENU, &MainFrame::OnMenuToolsAdvancedTroubleshoot, this, wxIdMenuToolsAdvancedTroubleshoot);
        toolsMenu->Append(wxID_ANY, _T("&Advanced"), toolsAdvancedMenu);

        menuBar->Append(toolsMenu, _T("&Tools"));
    }

    SetMenuBar(menuBar);

    rootPanel = new wxPanel(this);
    recentActiveConnectButton = GetInitPhaseDummyConnectButton();
    recentActiveDisconnectButton = GetInitPhaseDummyDisconnectButton();
    ipStaticText = new wxStaticText(rootPanel, wxID_ANY, _("Assigned IP"));
    recentActiveIpTextCtrl = GetInitPhaseDummyIpTextCtrl();
    recentActiveIpCopyAndRefreshButton = GetInitPhaseDummyIpCopyAndRefreshButton();
    recentActiveLiveLog = GetInitPhaseDummyLiveLog();

    currentTap_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Connect with"));
    currentTap_ComboBox = new wxComboBox(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
    ReloadCurrentTap();

    currentNetwork_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Network name"));
    currentNetwork_ComboBox = new wxComboBox(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
    currentNetwork_ComboBox->Bind(wxEVT_COMBOBOX, &MainFrame::OnCurrentNetworkChange, this);
    ReloadCurrentNetwork();

    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
}

void MainFrame::Init_Layout()
{
    namespace ls = Layout_SRV;

    SetSizeHints(640, 480);

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
        wxBoxSizer* networkControlRootSizer = new wxBoxSizer(wxHORIZONTAL);
        rootSizer->Add(networkControlRootSizer, 0, wxEXPAND);

        networkControlSizer = new wxBoxSizer(wxHORIZONTAL);
        networkControlRootSizer->Add(networkControlSizer, 1);
        networkControlSizer->Add(recentActiveConnectButton.get(), 1, wxLEFT, ls::SpaceToFrameBorder);
        networkControlSizer->Add(recentActiveDisconnectButton.get(), 1, wxLEFT, ls::SpaceBetweenControl);
        ls::AddFixedSpacer(wxRIGHT, 50 + ls::SpaceBetweenControl, networkControlSizer);

        wxBoxSizer* ipSizer = new wxBoxSizer(wxHORIZONTAL);
        networkControlRootSizer->Add(ipSizer, 1);
        ipSizer->Add(ipStaticText, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, ls::BoxSizerVhhOffset);
        ls::AddFixedSpacer(wxLEFT, ls::SpaceBetweenControl, ipSizer);
        ipSizer->Add(recentActiveIpTextCtrl.get(), ls::TakeAllSpace, wxRIGHT, ls::SpaceBetweenControl);
        ipSizer->Add(recentActiveIpCopyAndRefreshButton.get(), 0, wxRIGHT, ls::SpaceToFrameBorder);

        ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
    }

    {
        liveLogSizer = new wxBoxSizer(wxHORIZONTAL);
        rootSizer->Add(liveLogSizer, 1, wxEXPAND);
        liveLogSizer->Add(0, 0, 0, wxLEFT, ls::SpaceToFrameBorder);
        liveLogSizer->Add(recentActiveLiveLog.get(), 1, wxEXPAND);
        liveLogSizer->Add(0, 0, 0, wxRIGHT, ls::SpaceToFrameBorder);
    }

    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    Fit();
}

void MainFrame::Init_PostLayout()
{
    std::thread t1(&MainFrame::API_SRV_PostLayout, this);
    t1.detach();
}
