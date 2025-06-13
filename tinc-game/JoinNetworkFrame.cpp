#include "JoinNetworkFrame.h"
#include "Layout_SRV.h"

JoinNetworkFrame::JoinNetworkFrame(wxFrame* parentFrame) : wxFrame(parentFrame, wxID_ANY, _("Join network"))
{
    Init_CreateControls();
    Init_Layout();
}

void JoinNetworkFrame::Init_CreateControls()
{
    rootPanel = new wxPanel(this);
    joinByInviteCodePanel = new wxPanel(rootPanel);
    joinByInviteCodePanel->Hide();
    joinBy_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Join by"));
    joinBy_ComboBox = new wxComboBox(rootPanel, wxID_ANY);
    joinBy_ComboBox->Append(_("Invite code"));
    joinBy_ComboBox->SetSelection(static_cast<int>(JoinBy::InviteCode));

    {
        inviteCode_StaticText = new wxStaticText(joinByInviteCodePanel, wxID_ANY, _("Join by"));
        inviteCode_TextCtrl = new wxTextCtrl(joinByInviteCodePanel, wxID_ANY);
        saveTo_StaticText = new wxStaticText(joinByInviteCodePanel, wxID_ANY, _("Save to (network name)"));
        saveTo_ComboBox = new wxComboBox(joinByInviteCodePanel, wxID_ANY);
        saveTo_ComboBox->Append(L"Default");
    }
}

void JoinNetworkFrame::Init_Layout()
{
    namespace ls = Layout_SRV;

    this->SetSizeHints(290, 250);

    wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
    rootPanel->SetSizer(rootSizer);
    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    rootSizer->Add(joinBy_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
    rootSizer->Add(joinBy_ComboBox, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    this->Fit();
}
