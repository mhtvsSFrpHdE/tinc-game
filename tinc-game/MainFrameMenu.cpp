#include "MainFrame.h"
#include "EditNetworkFrame.h"
#include "JoinNetworkFrame.h"

bool AllAllowEdit(std::unordered_map<int, PerNetworkData>& data) {
    bool allAllowEdit = true;
    for (const auto& pair : data) {
        allAllowEdit = allAllowEdit && pair.second.allowEdit;
    }
    return allAllowEdit;
}

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
    bool allAllowEdit = AllAllowEdit(currentNetwork_ComboBox_RawData);
    if (allAllowEdit) {
        auto joinNetworkFrame = new JoinNetworkFrame(this);
        joinNetworkFrame->Center();
        joinNetworkFrame->Show();
    }
    else {
        wxMessageDialog(this, _("Disconnect ALL network before join new network")).ShowModal();
    }
}

void MainFrame::OnMenuNetworksRename(wxCommandEvent& event)
{
    bool allAllowEdit = AllAllowEdit(currentNetwork_ComboBox_RawData);
    wxMessageDialog(this, _("Rename")).ShowModal();
}

void MainFrame::OnMenuNetworksAdvancedDelete(wxCommandEvent& event)
{
    bool allAllowEdit = AllAllowEdit(currentNetwork_ComboBox_RawData);
    wxMessageDialog(this, _("Delete")).ShowModal();
}
