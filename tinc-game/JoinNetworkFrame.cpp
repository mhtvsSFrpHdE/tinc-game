#include "JoinNetworkFrame.h"
#include "Layout_SRV.h"
#include "String_SRV.h"
#include "Resource_SRV.h"
#include <thread>

JoinNetworkFrame::JoinNetworkFrame(wxFrame* parentFrame, std::function<void()> onCloseCallback) : wxFrame(parentFrame, wxID_ANY, _("Join network"))
{
    _parentFrame = parentFrame;
    _onCloseCallback = onCloseCallback;

    Init_CreateControls();
    Init_Layout();
}

void JoinNetworkFrame::API_UI_EndJoinNetworkByInviteCode(ReturnValue<JoinNetworkResult> result)
{
    if (result.success) {
        allowCloseFrame = true;
        wxMessageDialog(this, _("Successfully joined network")).ShowModal();
        Close();
    }
    else {
        if (result.returnBody.messageEnum == JoinNetworkResult::Enum::TincBinFailed) {
            wxMessageDialog(this, _("Failed to run tinc program")).ShowModal();
        }
        if (result.returnBody.messageEnum == JoinNetworkResult::Enum::InvalidInviteCode) {
            wxMessageDialog(this, _("Invalid invite code, check invite code format")).ShowModal();
        }
        if (result.returnBody.messageEnum == JoinNetworkResult::Enum::ConnectFailed) {
            wxMessageDialog(this, _("Failed to connect your server")).ShowModal();
        }
        if (result.returnBody.messageEnum == JoinNetworkResult::Enum::AuthenticateFailed) {
            wxMessageDialog(this, _("Server refused your invite code")).ShowModal();
        }
        if (result.returnBody.messageEnum == JoinNetworkResult::Enum::Other) {
            auto errorMessage = _("Unknown error") + String_SRV::newLine + result.returnBody.messageString;
            wxMessageDialog(this, errorMessage).ShowModal();
        }
    }

    joinByInviteCodePanel->Hide();
    retryPanel->Show();
    rootSizer->Layout();

    confirmButton->Enable(true);
    confirmButton->Hide();
    navigateSizer->Replace(confirmButton, retryButton);
    retryButton->Show();
    navigateSizer->Layout();

    cancelButton->Enable(true);
    joinBy_ComboBox->Enable(true);
    saveAs_ComboBox->Enable(true);
    inviteCode_TextCtrl->Enable(true);

    allowCloseFrame = true;
}

void JoinNetworkFrame::OnInviteCodeChanged(wxCommandEvent& event)
{
    namespace ss = String_SRV;

    auto text = inviteCode_TextCtrl->GetValue();
    if (text.Contains(ss::newLine) || text.Contains(ss::space)) {
        text.Replace(ss::newLine, wxEmptyString);
        text.Replace(ss::space, wxEmptyString);
        inviteCode_TextCtrl->SetValue(text);
        inviteCode_TextCtrl->SetInsertionPointEnd();
    }
}

void JoinNetworkFrame::OnConfirmButtonClick(wxCommandEvent& event)
{
    namespace ss = String_SRV;
    namespace rs = Resource_SRV;

    confirmButton->Enable(false);
    cancelButton->Enable(false);
    joinBy_ComboBox->Enable(false);
    saveAs_ComboBox->Enable(false);
    inviteCode_TextCtrl->Enable(false);

    auto networkName = saveAs_ComboBox->GetValue();
    if (networkName == wxEmptyString) {
        wxMessageDialog(this, _("Network name is required")).ShowModal();
        confirmButton->Enable(true);
        cancelButton->Enable(true);
        joinBy_ComboBox->Enable(true);
        saveAs_ComboBox->Enable(true);
        inviteCode_TextCtrl->Enable(true);
        return;
    }

    auto isValidFileOrDir = rs::IsValidFileOrDir(networkName);
    if (isValidFileOrDir.success == false) {
        auto hint = _("Network name must be a valid folder name")
            + ss::newLine + _("First discovered invalid character: ")
            + isValidFileOrDir.returnBody;
        wxMessageDialog(this, hint).ShowModal();
        confirmButton->Enable(true);
        cancelButton->Enable(true);
        joinBy_ComboBox->Enable(true);
        saveAs_ComboBox->Enable(true);
        inviteCode_TextCtrl->Enable(true);
        return;
    }

    auto networkDir = rs::Networks::GetNetworksDir();
    networkDir.AppendDir(networkName);
    if (networkDir.DirExists()) {
        wxMessageDialog(this, _("Network name already exist, use another one")).ShowModal();
        confirmButton->Enable(true);
        cancelButton->Enable(true);
        joinBy_ComboBox->Enable(true);
        saveAs_ComboBox->Enable(true);
        inviteCode_TextCtrl->Enable(true);
        return;
    }

    allowCloseFrame = false;
    auto inviteCode = inviteCode_TextCtrl->GetValue();
    std::thread t1(&JoinNetworkFrame::API_SRV_JoinNetworkByInviteCode, this, networkName.ToStdWstring(), inviteCode.ToStdWstring());
    t1.detach();
}

void JoinNetworkFrame::OnCancelButtonClick(wxCommandEvent& event)
{
    Close();
}

void JoinNetworkFrame::OnRetryButtonClick(wxCommandEvent& event)
{
}

void JoinNetworkFrame::Init_CreateControls()
{
    rootPanel = new wxPanel(this);
    joinBy_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Join by"));
    joinBy_ComboBox = new wxComboBox(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
    joinBy_ComboBox->Append(_("Invite code"));
    joinBy_ComboBox->SetSelection(static_cast<int>(JoinBy::InviteCode));
    saveAs_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Save as (network name)"));
    saveAs_ComboBox = new wxComboBox(rootPanel, wxID_ANY);
    saveAs_ComboBox->Append(L"Default");

    joinByInviteCodePanel = new wxPanel(rootPanel);
    {
        inviteCode_StaticText = new wxStaticText(joinByInviteCodePanel, wxID_ANY, _("Type invite code:"));
        inviteCode_TextCtrl = new wxTextCtrl(joinByInviteCodePanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
        inviteCode_TextCtrl->Bind(wxEVT_TEXT, &JoinNetworkFrame::OnInviteCodeChanged, this);
    }
    retryPanel = new wxPanel(rootPanel);
    {
        liveLog_StaticText = new wxStaticText(retryPanel, wxID_ANY, _("Error log:"));
        liveLog_TextCtrl = new wxTextCtrl(retryPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    }
    retryPanel->Hide();

    confirmButton = new wxButton(rootPanel, wxID_ANY, _("Join"));
    confirmButton->Bind(wxEVT_BUTTON, &JoinNetworkFrame::OnConfirmButtonClick, this);
    retryButton = new wxButton(rootPanel, wxID_ANY, _("Retry"));
    retryButton->Bind(wxEVT_BUTTON, &JoinNetworkFrame::OnRetryButtonClick, this);
    retryButton->Hide();
    cancelButton = new wxButton(rootPanel, wxID_ANY, _("Cancel"));
    cancelButton->Bind(wxEVT_BUTTON, &JoinNetworkFrame::OnCancelButtonClick, this);

    Bind(wxEVT_CLOSE_WINDOW, &JoinNetworkFrame::OnClose, this);
}

void JoinNetworkFrame::Init_Layout()
{
    namespace ls = Layout_SRV;

    this->SetSizeHints(640, 480);

    rootSizer = new wxBoxSizer(wxVERTICAL);
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
        saveAsSizer->Add(saveAs_ComboBox, 1, wxLEFT | wxEXPAND, ls::SpaceToFrameBorder);
        headerSizer->Add(saveAsSizer, 1, wxEXPAND);
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

    {
        wxBoxSizer* retrySizer = new wxBoxSizer(wxVERTICAL);
        retryPanel->SetSizer(retrySizer);
        retrySizer->Add(liveLog_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
        ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, retrySizer);

        wxBoxSizer* liveLogTextCtrlSizer = new wxBoxSizer(wxHORIZONTAL);
        retrySizer->Add(liveLogTextCtrlSizer, 1, wxEXPAND);
        liveLogTextCtrlSizer->Add(0, 0, 0, wxLEFT, ls::SpaceToFrameBorder);
        liveLogTextCtrlSizer->Add(liveLog_TextCtrl, ls::TakeAllSpace, wxEXPAND);
        liveLogTextCtrlSizer->Add(0, 0, 0, wxRIGHT, ls::SpaceToFrameBorder);
        ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, retrySizer);

        rootSizer->Add(retryPanel, 1, wxEXPAND);
    }

    navigateSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(navigateSizer);
    navigateSizer->AddStretchSpacer(ls::TakeAllSpace);
    navigateSizer->Add(confirmButton, 1, wxRIGHT, ls::SpaceBetweenControl);
    navigateSizer->Add(cancelButton, 1, wxRIGHT, ls::SpaceToFrameBorder);

    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    this->Fit();
}

void JoinNetworkFrame::OnClose(wxCloseEvent& event)
{
    if (allowCloseFrame == false) {
        return;
    }

    _onCloseCallback();
    _parentFrame->Raise();

    event.Skip();
}
