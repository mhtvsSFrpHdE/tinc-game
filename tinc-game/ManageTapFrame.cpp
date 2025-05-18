#include "ManageTapFrame.h"
#include "TapDevice_SRV.h"
#include "Layout_SRV.h"
#include "Style_SRV.h"
#include "String_SRV.h"

ManageTapFrame::ManageTapFrame(MainFrame* parentFrame) : wxFrame(parentFrame, wxID_ANY, _("Manage virtual network adapter"))
{
    TapDevice_SRV::ReloadAdapterList();

    Init_CreateControls();
    Init_Layout();
}

void ManageTapFrame::Init_CreateControls()
{
    rootPanel = new wxPanel(this);
    defaultTap_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Current default virtual network adapter:"));
    defaultTapValue_TextCtrl = new wxTextCtrl(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    Reload_defaultTapValue_TextCtrl();
    manageTap_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Manage virtual network adapter"));
    setAsDefault_Button = new wxButton(rootPanel, wxID_ANY, _("Set as default"));
    setAsDefault_Button->Enable(false);
    setAsDefault_Button->Bind(wxEVT_BUTTON, &ManageTapFrame::OnSetAsDefaultButtonClick, this);
    installTap_Button = new wxButton(rootPanel, wxID_ANY, _("Install new"));
    installTap_Button->Bind(wxEVT_BUTTON, &ManageTapFrame::OnInstallTapButtonClick, this);
    uninstallTapButton = new wxButton(rootPanel, wxID_ANY, _("Uninstall"));
    uninstallTapButton->Enable(false);
    uninstallTapButton->Bind(wxEVT_BUTTON, &ManageTapFrame::OnUninstallTapButtonClick, this);
    installedTap_ComboBox = new wxComboBox(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
    Reload_installedTap_ComboBox();
    installedTap_ComboBox->Bind(wxEVT_COMBOBOX, &ManageTapFrame::OnInstalledTapComboBoxChange, this);
    installedTapHelpMeDecide_Button = new wxButton(rootPanel, wxID_ANY, _("Help me decide"));
    installedTapHelpMeDecide_Button->Bind(wxEVT_BUTTON, &ManageTapFrame::OnInstalledTapHelpMeDecideButtonClick, this);
    installedTapRefresh_Button = new wxButton(rootPanel, wxID_ANY, _("Refresh"));
    installedTapRefresh_Button->Bind(wxEVT_BUTTON, &ManageTapFrame::OnInstalledTapRefreshButtonClick, this);
    closeButton = new wxButton(rootPanel, wxID_ANY, _("Close"));
    closeButton->Bind(wxEVT_BUTTON, &ManageTapFrame::OnCloseButtonClick, this);

    Bind(wxEVT_CLOSE_WINDOW, &ManageTapFrame::OnClose, this);
}

void ManageTapFrame::Init_Layout()
{
    namespace ls = Layout_SRV;

    this->SetSizeHints(405, 250);

    wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
    rootPanel->SetSizer(rootSizer);
    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    rootSizer->Add(defaultTap_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
    rootSizer->Add(defaultTapValue_TextCtrl, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    rootSizer->Add(manageTap_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    wxBoxSizer* installedTapSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(installedTapSizer);
    ls::AddFixedSpacer(wxLEFT, ls::SpaceToFrameBorder, installedTapSizer);
    installedTap_ComboBox->SetMinSize(wxSize(100, -1));
    installedTapSizer->Add(installedTap_ComboBox, ls::TakeAllSpace, wxRIGHT, ls::SpaceBetweenControl);
    installedTapSizer->Add(installedTapRefresh_Button, 1, wxRIGHT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    wxBoxSizer* manageTapSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(manageTapSizer);
    manageTapSizer->Add(installTap_Button, 0, wxLEFT, ls::SpaceToFrameBorder);
    manageTapSizer->Add(setAsDefault_Button, 0, wxLEFT, ls::SpaceBetweenControl);
    manageTapSizer->Add(uninstallTapButton, 0, wxLEFT, ls::SpaceBetweenControl);
    manageTapSizer->Add(installedTapHelpMeDecide_Button, 0, wxLEFT, ls::SpaceBetweenControl);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    wxBoxSizer* navigateSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(navigateSizer);
    navigateSizer->Add(0, 0, ls::TakeAllSpace);
    navigateSizer->Add(closeButton, 1, wxRIGHT, ls::SpaceToFrameBorder);

    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
}

void ManageTapFrame::OnClose(wxCloseEvent& event)
{
    if (allowCloseFrame == false) {
        return;
    }

    event.Skip();
}

void ManageTapFrame::OnInstalledTapComboBoxChange(wxCommandEvent& evt)
{
    auto hasValue = installedTap_ComboBox->GetSelection() >= 0;
    uninstallTapButton->Enable(hasValue);
    setAsDefault_Button->Enable(hasValue);
}

void ManageTapFrame::OnInstalledTapHelpMeDecideButtonClick(wxCommandEvent& evt)
{
    auto openNetworkControlPanel = TapDevice_SRV::API_SRV_OpenNetworkControlPanel();
    if (openNetworkControlPanel == false) {
        wxMessageDialog(this, TapDevice_SRV::openNetworkControlPanelFailedMessage).ShowModal();
    }
}

void ManageTapFrame::OnInstalledTapRefreshButtonClick(wxCommandEvent& evt)
{
    Reload();
}

void ManageTapFrame::OnSetAsDefaultButtonClick(wxCommandEvent& evt)
{
    auto selectedIndex = installedTap_ComboBox->GetSelection();
    auto adapter = installedTap_ComboBox_RawData[selectedIndex];
    TapDevice_SRV::SetDefaultTap(adapter);
    Reload_defaultTapValue_TextCtrl();
    wxMessageDialog(this, _("Successfully set default adapter to:") + String_SRV::newLine + adapter.friendlyName + " | " + adapter.modelName).ShowModal();
}

void ManageTapFrame::OnInstallTapButtonClick(wxCommandEvent& evt)
{
    allowCloseFrame = false;

    if (hasDefaultTap) {
        auto askResult = wxMessageBox(_("Detected exist virtual network adapter") + String_SRV::newLine + _("Install another one?"), wxEmptyString, wxYES_NO, this);
        if (askResult == wxNO)
        {
            allowCloseFrame = true;
            return;
        }
    }

    auto installTap = API_SRV_InstallTap();
    if (installTap.success) {
        wxMessageDialog(this, _("Successfully installed new adapter")).ShowModal();
    }
    else {
        wxMessageDialog(this, _("Install adapter failed:") + String_SRV::newLine + installTap.returnBody).ShowModal();
    }

    Reload();
    allowCloseFrame = true;
}

void ManageTapFrame::OnUninstallTapButtonClick(wxCommandEvent& evt)
{
    allowCloseFrame = false;

    auto getDefaultTap = TapDevice_SRV::GetDefaultTap();
    auto selectedIndex = installedTap_ComboBox->GetSelection();
    auto adapterToUninstall = installedTap_ComboBox_RawData[selectedIndex];
    bool requestClearDefaultTap = false;
    if (getDefaultTap.success) {
        bool uninstallingDefaultTap = getDefaultTap.returnBody.adapter.friendlyName == adapterToUninstall.friendlyName;
        if (uninstallingDefaultTap) {
            auto askResult = wxMessageBox(_("Uninstalling current default virtual network adapter") + String_SRV::newLine + _("Confirm?"), wxEmptyString, wxYES_NO, this);
            if (askResult == wxYES)
            {
                requestClearDefaultTap = true;
            }
            else {
                allowCloseFrame = true;
                return;
            }
        }
    }

    auto uninstallTap = API_SRV_UninstallTap(adapterToUninstall);
    if (uninstallTap.success) {
        wxMessageDialog(this, _("Successfully uninstalled adapter") + String_SRV::newLine + adapterToUninstall.friendlyName + " | " + adapterToUninstall.modelName).ShowModal();
    }
    else {
        wxMessageDialog(this, _("Uninstall adapter failed:") + String_SRV::newLine + uninstallTap.returnBody).ShowModal();
    }

    if (requestClearDefaultTap) {
        TapDevice_SRV::UnsetDefaultTap();
    }

    Reload();
    allowCloseFrame = true;
}

void ManageTapFrame::OnCloseButtonClick(wxCommandEvent& evt)
{
    Close();
}

void ManageTapFrame::Reload_installedTap_ComboBox()
{
    auto getNetworkAdapterList = TapDevice_SRV::API_SRV_GetNetworkAdapterList();
    if (getNetworkAdapterList.success) {
        installedTap_ComboBox->Clear();
        installedTap_ComboBox_RawData.clear();

        int mapIndex = 0;
        for (int adapterIndex = 0; adapterIndex < getNetworkAdapterList.returnBody.size(); adapterIndex++)
        {
            auto adapter = getNetworkAdapterList.returnBody[adapterIndex];
            bool isNotLoopback = adapter.isLoopback() == false;
            bool isTap = adapter.isTap();
            if (isNotLoopback && isTap) {
                installedTap_ComboBox_RawData.insert({ mapIndex, adapter });
                installedTap_ComboBox->Append(adapter.friendlyName + " | " + adapter.modelName);
                mapIndex = mapIndex + 1;
            }
        }
    }
}

void ManageTapFrame::Reload_defaultTapValue_TextCtrl()
{
    auto getTap = TapDevice_SRV::GetDefaultTap();
    if (getTap.success) {
        defaultTapValue_TextCtrl->SetLabel(getTap.returnBody.adapter.friendlyName);
        defaultTapValue_TextCtrl->SetBackgroundColour(Style_SRV::passed_green);
        hasDefaultTap = true;
    }
    else {
        defaultTapValue_TextCtrl->SetLabel(defaultTapValue_NoneText);
        defaultTapValue_TextCtrl->SetBackgroundColour(Style_SRV::denied_red);
        hasDefaultTap = false;
    }
}

void ManageTapFrame::Reload()
{
    TapDevice_SRV::ReloadAdapterList();
    Reload_defaultTapValue_TextCtrl();
    Reload_installedTap_ComboBox();
    setAsDefault_Button->Enable(false);
    uninstallTapButton->Enable(false);
}
