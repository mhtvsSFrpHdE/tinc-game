#include "boost/process.hpp"
#include <boost/process/windows.hpp>
#include "MainFrame.h"
#include "EditNetworkFrame.h"
#include "JoinNetworkFrame.h"
#include "String_SRV.h"
#include <wx/filename.h>
#include <wx/dir.h>
#include "RenameNetworkFrame.h"
#include "Resource_SRV.h"
#include "ManageTapFrame.h"
#include "SettingsFrame.h"
#include "OptimizeMtuFrame.h"
#include "HelpFrame.h"

bool MainFrame::AllowMakeChange(bool showDialog) {
    bool allAllowMakeChange = true;

    for (const auto& pair : currentNetwork_ComboBox_RawData) {
        allAllowMakeChange = allAllowMakeChange && pair.second.connected == false;
    }

    if (allAllowMakeChange == false && showDialog) {
        wxMessageDialog(this, _("Disconnect ALL network before making changes")).ShowModal();
    }

    return allAllowMakeChange;
}

void MainFrame::OnMenuNetworksEdit(wxCommandEvent& event)
{
    auto networkSelection = currentNetwork_ComboBox->GetSelection();
    auto& networkRawData = currentNetwork_ComboBox_RawData[networkSelection];

    if (networkRawData.connected) {
        wxMessageDialog(this, _("Disconnect network before edit: ") + networkRawData.network.networkName).ShowModal();
        return;
    }

    auto editNetworkFrame = new EditNetworkFrame(this, &networkRawData.network);
    editNetworkFrame->Center();
    editNetworkFrame->Show();
}

void MainFrame::OnJoinNetworkFrameCloseCallback()
{
    OnMenuNetworksReload_Internal();
}

void MainFrame::OnMenuNetworksJoin(wxCommandEvent& event)
{
    bool allAllowEdit = AllowMakeChange();
    if (allAllowEdit == false) {
        return;
    }

    std::function<void()> redirectCallback = std::bind(&MainFrame::OnJoinNetworkFrameCloseCallback, this);
    auto joinNetworkFrame = new JoinNetworkFrame(this, redirectCallback);
    joinNetworkFrame->Center();
    joinNetworkFrame->Show();
}

void MainFrame::OnRenameNetworkFrameCloseCallback()
{
    OnMenuNetworksReload_Internal();
}

void MainFrame::OnMenuNetworksRename(wxCommandEvent& event)
{
    bool allAllowEdit = AllowMakeChange();
    if (allAllowEdit == false) {
        return;
    }

    auto networkSelection = currentNetwork_ComboBox->GetSelection();
    auto& networkRawData = currentNetwork_ComboBox_RawData[networkSelection];

    std::function<void()> redirectCallback = std::bind(&MainFrame::OnRenameNetworkFrameCloseCallback, this);
    auto renameNetworkFrame = new RenameNetworkFrame(this, &networkRawData.network, redirectCallback);
    renameNetworkFrame->Center();
    renameNetworkFrame->Show();
}

void MainFrame::OnMenuNetworksImportAndExport(wxCommandEvent& event)
{
    namespace bp = boost::process;
    namespace rs = Resource_SRV;

    bool allAllowEdit = AllowMakeChange();
    if (allAllowEdit == false) {
        return;
    }

    wxMessageDialog(this, _("Run Networks - Reload after you are done")).ShowModal();

    bp::system(bp::shell(), bp::args({ rs::Bat::cmdRumCommand, L"explorer.exe", rs::Networks::networksDir.ToStdWstring() }), bp::windows::hide);
}

void MainFrame::OnMenuNetworksReload_Internal()
{
    allowCloseFrame = false;

    auto dummyConnectButton = GetInitPhaseDummyConnectButton();
    networkControlSizer->Replace(recentActiveConnectButton.get(), dummyConnectButton.get());
    recentActiveConnectButton = dummyConnectButton;
    auto dummyDisconnectButton = GetInitPhaseDummyDisconnectButton();
    networkControlSizer->Replace(recentActiveDisconnectButton.get(), dummyDisconnectButton.get());
    recentActiveDisconnectButton = dummyDisconnectButton;
    networkControlSizer->Layout();

    std::vector<std::wstring> existNetworks;
    for (size_t i = 0; i < currentNetwork_ComboBox_RawData.size(); i++)
    {
        existNetworks.push_back(currentNetwork_ComboBox_RawData[i].network.networkName);
    }

    ReloadCurrentTap();
    ReloadCurrentNetwork();

    std::vector<std::wstring> newNetworks;
    bool exactSameList = existNetworks.size() == currentNetwork_ComboBox_RawData.size();
    if (exactSameList) {
        for (size_t i = 0; i < currentNetwork_ComboBox_RawData.size(); i++)
        {
            auto& existNetworkName = existNetworks[i];
            auto& newNetworkName = currentNetwork_ComboBox_RawData[i].network.networkName;
            exactSameList = exactSameList && existNetworkName == newNetworkName;
        }
    }
    if (exactSameList) {
        currentNetwork_ComboBox->SetSelection(recentUsedNetworkSelection);
        currentNetwork_ComboBox->SendSelectionChangedEvent(wxEVT_COMBOBOX);
    }

    allowCloseFrame = true;
}

void MainFrame::OnMenuNetworksReload(wxCommandEvent& event)
{
    bool allAllowEdit = AllowMakeChange();
    if (allAllowEdit == false) {
        return;
    }

    OnMenuNetworksReload_Internal();
}

void MainFrame::OnMenuNetworksAdvancedDelete(wxCommandEvent& event)
{
    namespace ss = String_SRV;

    bool allAllowEdit = AllowMakeChange();
    if (allAllowEdit == false) {
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

    allowCloseFrame = false;

    auto networkSelection = currentNetwork_ComboBox->GetSelection();
    auto& networkRawData = currentNetwork_ComboBox_RawData[networkSelection];
    wxFileName::Rmdir(networkRawData.network.GetFullPath(), wxDIR_DIRS);

    OnMenuNetworksReload_Internal();
}

void MainFrame::OnManageTapFrameCloseCallback()
{
    OnMenuNetworksReload_Internal();
}

void MainFrame::OnMenuToolsManageTap(wxCommandEvent& event)
{
    bool allAllowEdit = AllowMakeChange();
    if (allAllowEdit == false) {
        return;
    }

    std::function<void()> redirectCallback = std::bind(&MainFrame::OnManageTapFrameCloseCallback, this);
    ManageTapFrame* manageTapDeviceFrame = new ManageTapFrame(this, redirectCallback);
    manageTapDeviceFrame->Center();
    manageTapDeviceFrame->Show();
}

void OnSettingsMenu_OpenSettingsFrame(MainFrame* mainFrame)
{
    SettingsFrame* settingsframe = new SettingsFrame(mainFrame);
    settingsframe->Center();
    settingsframe->Show();
}

void OnSettingsMenu_OtherWindowExists(MainFrame* mainFrame)
{
    wxString buttonHint = _("Close all windows before enter setting interface.");
    wxString title = _("hint");
    wxMessageDialog* dial = new wxMessageDialog(NULL,
        buttonHint, title, wxOK);
    dial->ShowModal();
}

void MainFrame::OnMenuToolsSettings(wxCommandEvent& event)
{
    bool allAllowEdit = AllowMakeChange();
    if (allAllowEdit == false) {
        return;
    }

    if (openedFrameCount == 0)
    {
        OnSettingsMenu_OpenSettingsFrame(this);
    }
    else {
        OnSettingsMenu_OtherWindowExists(this);
    }
}

void MainFrame::OnOptimizeMtuFrameCloseCallback()
{
    openedFrameCount--;
    toolsAdvancedMenu->Enable(wxIdMenuToolsAdvancedOptimizeMtu, true);
}

void MainFrame::OnMenuToolsAdvancedOptimizeMtu(wxCommandEvent& event)
{
    toolsAdvancedMenu->Enable(wxIdMenuToolsAdvancedOptimizeMtu, false);
    openedFrameCount++;

    std::function<void()> redirectCallback = std::bind(&MainFrame::OnOptimizeMtuFrameCloseCallback, this);
    OptimizeMtuFrame* optimizeMtuframe = new OptimizeMtuFrame(this, redirectCallback);
    optimizeMtuframe->Center();
    optimizeMtuframe->Show();
}

void MainFrame::OnTroubleshootFrameCloseCallback()
{
    toolsAdvancedMenu->Enable(wxIdMenuToolsAdvancedTroubleshoot, true);
}

void MainFrame::OnMenuToolsAdvancedTroubleshoot(wxCommandEvent& event)
{
    toolsAdvancedMenu->Enable(wxIdMenuToolsAdvancedTroubleshoot, false);

    auto checkResult = Resource_SRV::IntegrityCheck();

    std::function<void()> redirectCallback = std::bind(&MainFrame::OnTroubleshootFrameCloseCallback, this);
    HelpFrame* optimizeMtuFrame_HelpFrame = new HelpFrame(this, _("Troubleshoot"), redirectCallback);
    optimizeMtuFrame_HelpFrame->SetHelpText(checkResult);
    optimizeMtuFrame_HelpFrame->Center();
    optimizeMtuFrame_HelpFrame->Show();
}
