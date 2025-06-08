#include "EditNetworkFrame.h"
#include "String_SRV.h"
#include "Layout_SRV.h"
#include "Settings_SRV.h"

EditNetworkFrame::EditNetworkFrame(wxFrame* parentFrame, Networks_SRV::GetNetworksResult* network) : wxFrame(parentFrame, wxID_ANY, _("Edit:") + String_SRV::space + network->networkName)
{
    _network = network;

    Init_CreateControls();
    Init_Layout();
}

void EditNetworkFrame::Init_CreateControls()
{
    rootPanel = new wxPanel(this);
    gameModeCheckBox = new wxCheckBox(rootPanel, wxID_ANY, _("Game mode"));
    gameModeCheckBox->Bind(wxEVT_CHECKBOX, &EditNetworkFrame::OnGameModeCheckBoxClick, this);
    autoConnectOnStartCheckBox = new wxCheckBox(rootPanel, wxID_ANY, _("Auto connect on start"));
    showDetailedLiveLogCheckBox = new wxCheckBox(rootPanel, wxID_ANY, _("Show detailed live log"));
    {
        auto verboseSettingsKey = SettingKeys_Networks::network_verbose(_network->networkName);
        auto verbose = Settings_SRV::networksConfig->ReadBool(verboseSettingsKey, true);
        showDetailedLiveLogCheckBox->SetValue(verbose);

        auto gameModeSettingsKey = SettingKeys_Networks::network_gameMode(_network->networkName);
        auto gameMode = Settings_SRV::networksConfig->ReadBool(gameModeSettingsKey, false);
        gameModeCheckBox->SetValue(gameMode);

        auto autoStartSettingsKey = SettingKeys_Networks::network_autoStart(_network->networkName);
        auto autoStart = Settings_SRV::networksConfig->ReadBool(autoStartSettingsKey, false);
        autoConnectOnStartCheckBox->SetValue(autoStart);
    }
    confirmButton = new wxButton(rootPanel, wxID_ANY, _("Confirm"));
    confirmButton->Bind(wxEVT_BUTTON, &EditNetworkFrame::OnConfirmButtonClick, this);
    cancelButton = new wxButton(rootPanel, wxID_ANY, _("Cancel"));
    cancelButton->Bind(wxEVT_BUTTON, &EditNetworkFrame::OnCancelButtonClick, this);
}

void EditNetworkFrame::Init_Layout()
{
    namespace ls = Layout_SRV;

    this->SetSizeHints(290, 190);

    wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
    rootPanel->SetSizer(rootSizer);
    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    rootSizer->Add(gameModeCheckBox, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
    rootSizer->Add(autoConnectOnStartCheckBox, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
    rootSizer->Add(showDetailedLiveLogCheckBox, 0, wxLEFT, ls::SpaceToFrameBorder);
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
    }
}

void EditNetworkFrame::OnConfirmButtonClick(wxCommandEvent& event)
{
    auto verboseSettingsKey = SettingKeys_Networks::network_verbose(_network->networkName);
    auto verbose = showDetailedLiveLogCheckBox->GetValue();
    Settings_SRV::networksConfig->Write(verboseSettingsKey, verbose);

    auto gameModeSettingsKey = SettingKeys_Networks::network_gameMode(_network->networkName);
    auto gameMode = gameModeCheckBox->GetValue();
    Settings_SRV::networksConfig->Write(gameModeSettingsKey, gameMode);

    auto autoStartSettingsKey = SettingKeys_Networks::network_autoStart(_network->networkName);
    auto autoStart = autoConnectOnStartCheckBox->GetValue();
    Settings_SRV::networksConfig->Write(autoStartSettingsKey, autoStart);

    Settings_SRV::networksConfig->Flush();
    Close();
}

void EditNetworkFrame::OnCancelButtonClick(wxCommandEvent& event)
{
    Close();
}
