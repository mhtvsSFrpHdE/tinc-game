#include "RenameNetworkFrame.h"
#include "String_SRV.h"
#include "Layout_SRV.h"
#include "Resource_SRV.h"

RenameNetworkFrame::RenameNetworkFrame(wxFrame* parentFrame, Networks_SRV::GetNetworksResult* network) : wxFrame(parentFrame, wxID_ANY, _("Rename:") + String_SRV::space + network->networkName)
{
    _network = network;

    Init_CreateControls();
    Init_Layout();
}

void RenameNetworkFrame::Init_CreateControls()
{
    rootPanel = new wxPanel(this);

    newName_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Rename to:"));
    newName_ComboBox = new wxComboBox(rootPanel, wxID_ANY);
    {
        newName_ComboBox->Append("Default");
    }
    confirmButton = new wxButton(rootPanel, wxID_ANY, _("Confirm"));
    confirmButton->Bind(wxEVT_BUTTON, &RenameNetworkFrame::OnConfirmButtonClick, this);
    cancelButton = new wxButton(rootPanel, wxID_ANY, _("Cancel"));
    cancelButton->Bind(wxEVT_BUTTON, &RenameNetworkFrame::OnCancelButtonClick, this);
}

void RenameNetworkFrame::Init_Layout()
{
    namespace ls = Layout_SRV;

    this->SetSizeHints(290, 250);

    wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
    rootPanel->SetSizer(rootSizer);
    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    rootSizer->Add(newName_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
    rootSizer->Add(newName_ComboBox, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    rootSizer->Add(0, 0, ls::TakeAllSpace, wxEXPAND);
    wxBoxSizer* navigateSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(navigateSizer);
    navigateSizer->AddStretchSpacer(ls::TakeAllSpace);
    navigateSizer->Add(confirmButton, 1, wxRIGHT, ls::SpaceBetweenControl);
    navigateSizer->Add(cancelButton, 1, wxRIGHT, ls::SpaceToFrameBorder);

    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);
}

void RenameNetworkFrame::OnConfirmButtonClick(wxCommandEvent& event)
{
    namespace ss = String_SRV;
    namespace rs = Resource_SRV;

    confirmButton->Enable(false);
    cancelButton->Enable(false);

    auto networkName = newName_ComboBox->GetValue();
    if (networkName == wxEmptyString) {
        wxMessageDialog(this, _("New network name is required")).ShowModal();
        confirmButton->Enable(true);
        cancelButton->Enable(true);
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
        return;
    }

    auto newNetworkDir = rs::Networks::GetNetworksDir();
    newNetworkDir.AppendDir(networkName);
    if (newNetworkDir.DirExists()) {
        wxMessageDialog(this, _("Network name already exist, use another one")).ShowModal();
        confirmButton->Enable(true);
        cancelButton->Enable(true);
        return;
    }

    auto existNetworkDir = rs::Networks::GetNetworksDir();
    existNetworkDir.AppendDir(_network->networkName);
    bool result = wxRenameFile(existNetworkDir.GetFullPath(), newNetworkDir.GetFullPath());
    if (result) {
        Close();
    }
}

void RenameNetworkFrame::OnCancelButtonClick(wxCommandEvent& event)
{
    Close();
}
