#include "EditNetworkFrame.h"
#include "String_SRV.h"
#include "Layout_SRV.h"

EditNetworkFrame::EditNetworkFrame(wxFrame* parentFrame, Networks_SRV::GetNetworksResult& network) : wxFrame(parentFrame, wxID_ANY, _("Edit:") + String_SRV::space + network.networkName)
{
    Init_CreateControls();
    Init_Layout();
}

void EditNetworkFrame::Init_CreateControls()
{
    rootPanel = new wxPanel(this);
    gameModeCheckBox = new wxCheckBox(rootPanel, wxID_ANY, _("Game mode"));
    autoConnectOnStartCheckBox = new wxCheckBox(rootPanel, wxID_ANY, _("Auto connect on start"));
    confirmButton = new wxButton(rootPanel, wxID_ANY, _("Confirm"));
    confirmButton->Bind(wxEVT_BUTTON, &EditNetworkFrame::OnConfirmButtonClick, this);
    cancelButton = new wxButton(rootPanel, wxID_ANY, _("Cancel"));
    cancelButton->Bind(wxEVT_BUTTON, &EditNetworkFrame::OnCancelButtonClick, this);
}

void EditNetworkFrame::Init_Layout()
{
    namespace ls = Layout_SRV;

    this->SetSizeHints(290, 160);

    wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
    rootPanel->SetSizer(rootSizer);
    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    rootSizer->Add(gameModeCheckBox, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
    rootSizer->Add(autoConnectOnStartCheckBox, 0, wxLEFT, ls::SpaceToFrameBorder);
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

void EditNetworkFrame::OnConfirmButtonClick(wxCommandEvent& event)
{
}

void EditNetworkFrame::OnCancelButtonClick(wxCommandEvent& event)
{
    Close();
}
