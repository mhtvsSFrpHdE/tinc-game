#include "JoinNetworkFrame.h"
#include "Layout_SRV.h"

JoinNetworkFrame::JoinNetworkFrame(wxFrame* parentFrame) : wxFrame(parentFrame, wxID_ANY, _("Join network"))
{
    Init_CreateControls();
    Init_Layout();
}

void JoinNetworkFrame::OnJoinButtonClick(wxCommandEvent& event)
{
}

void JoinNetworkFrame::OnCloseButtonClick(wxCommandEvent& event)
{
    Close();
}

void JoinNetworkFrame::Init_CreateControls()
{
    rootPanel = new wxPanel(this);
    joinByInviteCodePanel = new wxPanel(rootPanel);
    joinBy_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Join by"));
    joinBy_ComboBox = new wxComboBox(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
    joinBy_ComboBox->Append(_("Invite code"));
    joinBy_ComboBox->SetSelection(static_cast<int>(JoinBy::InviteCode));
    saveAs_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Save as (network name)"));
    saveAs_ComboBox = new wxComboBox(rootPanel, wxID_ANY);
    saveAs_ComboBox->Append(L"Default");

    {
        inviteCode_StaticText = new wxStaticText(joinByInviteCodePanel, wxID_ANY, _("Invite code"));
        inviteCode_TextCtrl = new wxTextCtrl(joinByInviteCodePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    }

    joinButton = new wxButton(rootPanel, wxID_ANY, _("Confirm"));
    joinButton->Bind(wxEVT_BUTTON, &JoinNetworkFrame::OnJoinButtonClick, this);
    closeButton = new wxButton(rootPanel, wxID_ANY, _("Close"));
    closeButton->Bind(wxEVT_BUTTON, &JoinNetworkFrame::OnCloseButtonClick, this);
}

void JoinNetworkFrame::Init_Layout()
{
    namespace ls = Layout_SRV;

    this->SetSizeHints(640, 480);

    wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
    rootPanel->SetSizer(rootSizer);
    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    wxBoxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);
    {
        wxBoxSizer* joinBySizer = new wxBoxSizer(wxVERTICAL);
        joinBySizer->Add(joinBy_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
        ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, joinBySizer);
        joinBySizer->Add(joinBy_ComboBox, 0, wxLEFT, ls::SpaceToFrameBorder);
        headerSizer->Add(joinBySizer);

        wxBoxSizer* saveAsSizer = new wxBoxSizer(wxVERTICAL);
        saveAsSizer->Add(saveAs_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
        ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, saveAsSizer);
        saveAsSizer->Add(saveAs_ComboBox, 0, wxLEFT, ls::SpaceToFrameBorder);
        headerSizer->Add(saveAsSizer);
    }
    rootSizer->Add(headerSizer);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    {
        wxBoxSizer* joinByInviteCodeSizer = new wxBoxSizer(wxVERTICAL);
        joinByInviteCodePanel->SetSizer(joinByInviteCodeSizer);
        joinByInviteCodeSizer->Add(inviteCode_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
        ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, joinByInviteCodeSizer);

        wxBoxSizer* inviteCodeTextCtrlSizer = new wxBoxSizer(wxHORIZONTAL);
        joinByInviteCodeSizer->Add(inviteCodeTextCtrlSizer, 1, wxEXPAND);
        inviteCodeTextCtrlSizer->Add(0, 0, 0, wxLEFT, ls::SpaceToFrameBorder);
        inviteCodeTextCtrlSizer->Add(inviteCode_TextCtrl, ls::TakeAllSpace, wxEXPAND);
        inviteCodeTextCtrlSizer->Add(0, 0, 0, wxRIGHT, ls::SpaceToFrameBorder);
        ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, joinByInviteCodeSizer);

        rootSizer->Add(joinByInviteCodePanel, 1, wxEXPAND);
    }

    wxBoxSizer* navigateSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(navigateSizer);
    navigateSizer->AddStretchSpacer(ls::TakeAllSpace);
    navigateSizer->Add(joinButton, 1, wxRIGHT, ls::SpaceBetweenControl);
    navigateSizer->Add(closeButton, 1, wxRIGHT, ls::SpaceToFrameBorder);

    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    this->Fit();
}
