#include "MainFrame.h"
#include "EditNetworkFrame.h"
#include "JoinNetworkFrame.h"
#include "String_SRV.h"
#include <wx/filename.h>
#include <wx/dir.h>

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
    namespace ss = String_SRV;

    bool allAllowEdit = AllAllowEdit(currentNetwork_ComboBox_RawData);
    if (allAllowEdit == false) {
        wxMessageDialog(this, _("Disconnect ALL network before delete any network")).ShowModal();
        return;
    }

    auto firstHint = _("Your network password (identity) only stored on your local computer")
        + ss::newLine
        + ss::newLine + _("I can not help you recover any of your data")
        + ss::newLine + _("To join the network again, you may need another invite code")
        + ss::newLine
        + ss::newLine + _("Confirm delete network?");
    auto firstAskResult = wxMessageBox(firstHint, _("WARNING"), wxYES_NO | wxICON_ERROR, this);
    if (firstAskResult == wxNO)
    {
        return;
    }

    auto secondHint = _("Confirm delete network?");
    auto secondAskResult = wxMessageBox(secondHint, _("WARNING"), wxYES_NO | wxICON_ERROR, this);
    if (secondAskResult == wxNO)
    {
        return;
    }

    auto networkSelection = currentNetwork_ComboBox->GetSelection();
    auto& networkRawData = currentNetwork_ComboBox_RawData[networkSelection];
    wxFileName::Rmdir(networkRawData.network.GetFullPath(), wxDIR_DIRS);
}
