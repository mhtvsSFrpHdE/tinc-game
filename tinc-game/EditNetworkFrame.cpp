#include "EditNetworkFrame.h"
#include "String_SRV.h"
#include "Layout_SRV.h"
#include "Settings_SRV.h"
#include <sstream>
#include "HelpFrame.h"

EditNetworkFrame::EditNetworkFrame(wxFrame* parentFrame, Networks_SRV::GetNetworksResult* network) : wxFrame(parentFrame, wxID_ANY, _("Edit:") + String_SRV::space + network->networkName)
{
    _parentFrame = parentFrame;
    _network = network;

    Init_CreateControls();
    Init_Layout();
}

void EditNetworkFrame::Init_CreateControls()
{
    namespace ss = Settings_SRV;

    rootPanel = new wxPanel(this);
    helpButton = new wxButton(rootPanel, wxID_ANY, _("Help"));
    helpButton->Bind(wxEVT_BUTTON, &EditNetworkFrame::OnHelpButtonClick, this);
    gameModeCheckBox = new wxCheckBox(rootPanel, wxID_ANY, _("Game mode"));
    gameModeCheckBox->Bind(wxEVT_CHECKBOX, &EditNetworkFrame::OnGameModeCheckBoxClick, this);
    {
        auto gameModeSettingsKey = SettingKeys_Networks::network_gameMode(_network->networkName);
        auto gameMode = ss::networksConfig->ReadBool(gameModeSettingsKey, false);
        gameModeCheckBox->SetValue(gameMode);
    }
    autoConnectOnStartCheckBox = new wxCheckBox(rootPanel, wxID_ANY, _("Auto connect on start"));
    {
        auto autoStartSettingsKey = SettingKeys_Networks::network_autoStart(_network->networkName);
        auto autoStart = ss::networksConfig->ReadBool(autoStartSettingsKey, false);
        autoConnectOnStartCheckBox->SetValue(autoStart);
    }
    showDetailedLiveLogCheckBox = new wxCheckBox(rootPanel, wxID_ANY, _("Show detailed live log"));
    {
        auto verboseSettingsKey = SettingKeys_Networks::network_verbose(_network->networkName);
        auto verbose = ss::networksConfig->ReadBool(verboseSettingsKey, true);
        showDetailedLiveLogCheckBox->SetValue(verbose);
    }
    setMetricCheckBox = new wxCheckBox(rootPanel, wxID_ANY, _("Set metric before connect"));
    {
        auto setMetricSettingsKey = SettingKeys_Networks::network_setMetric(_network->networkName);
        auto setMetric = ss::networksConfig->ReadBool(setMetricSettingsKey, true);
        setMetricCheckBox->SetValue(setMetric);
    }
    portNumber_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Port (0-65535):"));
    portNumber_ComboBox = new wxComboBox(rootPanel, wxID_ANY);
    {
        portNumber_ComboBox->Append(_("0"));

        auto portSettingsKey = SettingKeys_Networks::network_port(_network->networkName);
        auto port = ss::networksConfig->Read(portSettingsKey);
        portNumber_ComboBox->SetValue(port);
    }
    confirmButton = new wxButton(rootPanel, wxID_ANY, _("Confirm"));
    confirmButton->Bind(wxEVT_BUTTON, &EditNetworkFrame::OnConfirmButtonClick, this);
    cancelButton = new wxButton(rootPanel, wxID_ANY, _("Cancel"));
    cancelButton->Bind(wxEVT_BUTTON, &EditNetworkFrame::OnCancelButtonClick, this);

    Bind(wxEVT_CLOSE_WINDOW, &EditNetworkFrame::OnClose, this);
}

void EditNetworkFrame::Init_Layout()
{
    namespace ls = Layout_SRV;

    this->SetSizeHints(290, 250);

    wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
    rootPanel->SetSizer(rootSizer);
    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    wxBoxSizer* helpSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(helpSizer);
    helpSizer->Add(gameModeCheckBox, 1, wxLEFT | wxALIGN_BOTTOM, ls::SpaceToFrameBorder);
    helpSizer->AddStretchSpacer(ls::TakeAllSpace);
    helpSizer->Add(helpButton, 1, wxRIGHT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    rootSizer->Add(autoConnectOnStartCheckBox, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
    rootSizer->Add(showDetailedLiveLogCheckBox, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
    rootSizer->Add(setMetricCheckBox, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    wxBoxSizer* portSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(portSizer);
    ls::AddFixedSpacer(wxLEFT, ls::SpaceToFrameBorder, portSizer);
    portSizer->Add(portNumber_StaticText, 1, wxRIGHT, ls::SpaceBetweenControl);
    portSizer->Add(portNumber_ComboBox, ls::TakeAllSpace, wxRIGHT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    rootSizer->Add(0, 0, ls::TakeAllSpace, wxEXPAND);
    wxBoxSizer* navigateSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(navigateSizer);
    navigateSizer->AddStretchSpacer(ls::TakeAllSpace);
    navigateSizer->Add(confirmButton, 1, wxRIGHT, ls::SpaceBetweenControl);
    navigateSizer->Add(cancelButton, 1, wxRIGHT, ls::SpaceToFrameBorder);

    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    this->Fit();
}

void EditNetworkFrame::OnClose(wxCloseEvent& event)
{
    _parentFrame->Raise();

    event.Skip();
}

void EditNetworkFrame::OnHelpButtonClick(wxCommandEvent& event)
{
    namespace ss = String_SRV;

    std::ostringstream helpTextStream;
    helpTextStream
        << _("Game mode")
        << ss::newLine
        << _("Allow you to utilize potential high quality internet lane for reducing latency and packet lose rate, carefully read hints when click on it, and try your best to NOT to use this feature, unless your game is really latency sensitive or your ISP is totally crap")
        << ss::newLine << ss::newLine
        << _("Auto connect on start")
        << ss::newLine
        << _("Manually connect your network once before enable auto connect to let tinc-game know relationship between network and virtual network adapter, otherwise each network will try to use first available virtual network adapter")
        << ss::newLine << ss::newLine
        << _("Show detailed live log")
        << ss::newLine
        << _("Append \"--debug=3\" to tincd start argument, print more network activity information like users you are connecting to, and their public IP address")
        << ss::newLine << ss::newLine
        << _("Set metric before connect")
        << ss::newLine
        << _("Some router push private network route table like 10.x.x.x to device connected to it for unknown reason, cause tinc virtual network inaccessible. Set metric=1 before connect can workaround this. If you don't like set each time on connect, go to \"Manage virtual network adapter\" and there is a button let you persistent set metric, so you can turn off option here and connect to network even faster")
        << ss::newLine << ss::newLine
        << _("Port")
        << ss::newLine
        << _("If you don't have open port and just a regular player, set to 0 to use random port to increase UDP hole punching success rate. If you have dedicated public IPv4 address and open port, you can set your port forwarding information here, let other people connect to you in direct path to minimize lantancy, this WILL ALSO allow other people do UDP hole punching between each other by using your network to swap information, or even forward traffic through your network when UDP hole punching failed. tinc didn't provide an option to turn off \"help other people\", so you can't have open port but refuse to serve others at same time, consider to limit your upload speed to prevent your upload traffic goes wild by using tools like NetLimiter on Windows, nftables on Linux");

    std::function<void()> redirectCallback = std::bind(&EditNetworkFrame::OnHelpFrameCloseCallback, this);
    HelpFrame* helpFrame = new HelpFrame(this, _("About network settings"), redirectCallback);
    helpFrame->SetHelpText(helpTextStream.str());
    helpFrame->Center();
    helpFrame->Show();

    helpButton->Enable(false);
}

void EditNetworkFrame::OnHelpFrameCloseCallback()
{
    helpButton->Enable(true);
}

void EditNetworkFrame::OnGameModeCheckBoxClick(wxCommandEvent& event)
{
    namespace ss = String_SRV;

    auto value = gameModeCheckBox->GetValue();
    if (value) {
        auto hint = _("You swear to use Game Mode only for playing games")
            + ss::newLine
            + ss::newLine + _("Enable game mode may brings lower network lantency and less packet lose")
            + ss::newLine + _("But high quality internet lane costs more and budget is limited")
            + ss::newLine + _("Especally when you run too much traffic on free service may get your account banned")
            + ss::newLine
            + ss::newLine + _("Games regularly cost less than 100 MB per hour")
            + ss::newLine + _("As long as you are truly run games, data usage won't go wild, no one will be hurted")
            + ss::newLine + _("Certain game like Garry's Mod are exception, they can cost hundureds GB instead")
            + ss::newLine + _("In this case, please disable Game Mode");
        auto askResult = wxMessageBox(hint, _("WARNING"), wxYES_NO | wxICON_ERROR, this);
        if (askResult == wxNO)
        {
            gameModeCheckBox->SetValue(false);
            return;
        }
        portNumber_ComboBox->SetValue(wxT("0"));
    }
}

void EditNetworkFrame::OnConfirmButtonClick(wxCommandEvent& event)
{
    namespace ss = Settings_SRV;

    auto portSettingsKey = SettingKeys_Networks::network_port(_network->networkName);
    auto port = portNumber_ComboBox->GetValue();
    auto gameMode = gameModeCheckBox->GetValue();
    {
        auto invalidPortHint = _("Invalid port number");
        try {
            auto portInt = std::stoi(port.ToStdWstring());
            auto validPort = portInt >= 0 && portInt <= 65535;
            if (validPort == false) {
                wxMessageDialog(this, invalidPortHint).ShowModal();
                return;
            }
            if (gameMode && portInt != 0)
            {
                wxMessageDialog(this, _("Game Mode has enabled, port must set to 0")).ShowModal();
                return;
            }
        }
        catch (...) {
            wxMessageDialog(this, invalidPortHint).ShowModal();
            return;
        }
    }
    ss::networksConfig->Write(portSettingsKey, port);

    auto verboseSettingsKey = SettingKeys_Networks::network_verbose(_network->networkName);
    auto verbose = showDetailedLiveLogCheckBox->GetValue();
    ss::networksConfig->Write(verboseSettingsKey, verbose);

    auto setMetricSettingsKey = SettingKeys_Networks::network_setMetric(_network->networkName);
    auto setMetric = setMetricCheckBox->GetValue();
    ss::networksConfig->Write(setMetricSettingsKey, setMetric);

    auto gameModeSettingsKey = SettingKeys_Networks::network_gameMode(_network->networkName);
    ss::networksConfig->Write(gameModeSettingsKey, gameMode);

    auto autoStartSettingsKey = SettingKeys_Networks::network_autoStart(_network->networkName);
    auto autoStart = autoConnectOnStartCheckBox->GetValue();
    ss::networksConfig->Write(autoStartSettingsKey, autoStart);

    ss::networksConfig->Flush();
    Close();
}

void EditNetworkFrame::OnCancelButtonClick(wxCommandEvent& event)
{
    Close();
}
