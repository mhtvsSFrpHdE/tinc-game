#include "EditNetworkFrame.h"
#include "String_SRV.h"

EditNetworkFrame::EditNetworkFrame(wxFrame* parentFrame, Networks_SRV::GetNetworksResult& network) : wxFrame(parentFrame, wxID_ANY, _("Edit:") + String_SRV::space + network.networkName)
{
    Init_CreateControls();
    Init_Layout();
}

void EditNetworkFrame::Init_CreateControls()
{
    rootPanel = new wxPanel(this);
}

void EditNetworkFrame::Init_Layout()
{
}

void EditNetworkFrame::OnConfirmButtonClick(wxCommandEvent& event)
{
}