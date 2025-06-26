#include "MainFrame.h"
#include "EditNetworkFrame.h"

void MainFrame::OnMenuNetworksEdit(wxCommandEvent& event)
{
    auto networkSelection = currentNetwork_ComboBox->GetSelection();
    auto& networkRawData = currentNetwork_ComboBox_RawData[networkSelection];

    if (networkRawData.allowEdit) {
        auto editNetworkFrame = new EditNetworkFrame(this, &networkRawData.network);
        editNetworkFrame->Center();
        editNetworkFrame->Show();
    }
    else {
        wxMessageDialog(this, _("Disconnect network before edit: ") + networkRawData.network.networkName).ShowModal();
    }
}

void MainFrame::OnMenuNetworksJoin(wxCommandEvent& event)
{
    wxMessageDialog(this, _("Join network")).ShowModal();
}

void MainFrame::OnMenuNetworksRename(wxCommandEvent& event)
{
    wxMessageDialog(this, _("Rename")).ShowModal();
}

void MainFrame::OnMenuNetworksAdvancedDelete(wxCommandEvent& event)
{
    wxMessageDialog(this, _("Delete")).ShowModal();
}
