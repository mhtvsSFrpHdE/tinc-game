#include "ManageTapFrame.h"
#include "TapDevice_SRV.h"
#include "Layout_SRV.h"
#include "Style_SRV.h"
#include "String_SRV.h"

ManageTapFrame::ManageTapFrame(MainFrame* parentFrame) : wxFrame(parentFrame, wxID_ANY, _("Manage virtual network adapter"))
{
    Init_CreateControls();
    Init_BindEventHandlers();
    Init_Layout();
}

ReturnValue<std::vector<WindowsAPI_SRV::GetAdaptersAddressesResult>> ManageTapFrame::API_SRV_GetNetworkAdapterList()
{
    auto result = ReturnValue<std::vector<WindowsAPI_SRV::GetAdaptersAddressesResult>>();

    WindowsAPI_SRV::GetAdaptersAddresses(&result.returnBody);

    if (result.returnBody.size() > 0) {
        result.success = true;
    }

    return result;
}

ReturnValue<std::wstring> ManageTapFrame::API_SRV_InstallTap()
{
    auto result = ReturnValue<std::wstring>();
    result.success = false;
    result.returnBody = L"Unknown";
    return result;
}

ReturnValue<std::wstring> ManageTapFrame::API_SRV_UninstallTap(WindowsAPI_SRV::GetAdaptersAddressesResult adapter)
{
    auto result = ReturnValue<std::wstring>();
    result.returnBody = adapter.friendlyName + " | " + adapter.modelName + String_SRV::newLine + adapter.windows_LUID;
    result.success = true;
    return result;
}

void ManageTapFrame::Init_CreateControls()
{
    rootPanel = new wxPanel(this);
    defaultTap_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Current default virtual network adapter:"));
    defaultTapValue_TextCtrl = new wxTextCtrl(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    Reload_defaultTapValue_TextCtrl();
    manageTap_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Manage virtual network adapter"));
    installTap_Button = new wxButton(rootPanel, wxID_ANY, _("Install new"));
    installedTap_ComboBox = new wxComboBox(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
    Reload_installedTap_ComboBox();
    uninstallTapButton = new wxButton(rootPanel, wxID_ANY, _("Uninstall selected"));
    uninstallTapButton->Enable(false);
    closeButton = new wxButton(rootPanel, wxID_ANY, _("Close"));
}

void ManageTapFrame::Init_BindEventHandlers()
{
    Bind(wxEVT_CLOSE_WINDOW, &ManageTapFrame::OnClose, this);
    installedTap_ComboBox->Bind(wxEVT_COMBOBOX, &ManageTapFrame::OnInstalledTapComboBoxChange, this);
    installTap_Button->Bind(wxEVT_BUTTON, &ManageTapFrame::OnInstallTapButtonClick, this);
    uninstallTapButton->Bind(wxEVT_BUTTON, &ManageTapFrame::OnUninstallTapButtonClick, this);
    closeButton->Bind(wxEVT_BUTTON, &ManageTapFrame::OnCloseButtonClick, this);
}

void ManageTapFrame::Init_Layout()
{
    namespace ls = Layout_SRV;

    this->SetSizeHints(320, 250);

    wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
    rootPanel->SetSizer(rootSizer);
    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    rootSizer->Add(defaultTap_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
    rootSizer->Add(defaultTapValue_TextCtrl, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    rootSizer->Add(manageTap_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
    rootSizer->Add(installedTap_ComboBox, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    wxBoxSizer* manageTapSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(manageTapSizer);
    manageTapSizer->Add(installTap_Button, 0, wxLEFT, ls::SpaceToFrameBorder);
    manageTapSizer->Add(uninstallTapButton, 0, wxLEFT, ls::SpaceBetweenControl);
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
}

void ManageTapFrame::OnInstallTapButtonClick(wxCommandEvent& evt)
{
    allowCloseFrame = false;

    if (hasDefaultTap) {
        auto askResult = wxMessageBox(_("Detected exist virtual network adapter") + String_SRV::newLine + _("Install another one?"), wxEmptyString, wxYES_NO, this);
        if (askResult == wxYES)
        {
            auto installTap = API_SRV_InstallTap();
            if (installTap.success) {
                wxMessageDialog(this, _("Successfully installed new adapter")).ShowModal();
            }
            else {
                wxMessageDialog(this, _("Install adapter failed:") + String_SRV::newLine + installTap.returnBody).ShowModal();
            }
        }
    }

    Reload();
    allowCloseFrame = true;
}

void ManageTapFrame::OnUninstallTapButtonClick(wxCommandEvent& evt)
{
    allowCloseFrame = false;

    auto selectedIndex = installedTap_ComboBox->GetSelection();
    auto adapter = installedTap_ComboBox_RawData[selectedIndex];
    auto uninstallTap = API_SRV_UninstallTap(adapter);
    if (uninstallTap.success) {
        wxMessageDialog(this, _("Successfully uninstalled adapter") + String_SRV::newLine + adapter.friendlyName + " | " + adapter.modelName).ShowModal();
    }
    else {
        wxMessageDialog(this, _("Uninstall adapter failed:") + String_SRV::newLine + uninstallTap.returnBody).ShowModal();
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
    auto getNetworkAdapterList = API_SRV_GetNetworkAdapterList();
    if (getNetworkAdapterList.success) {
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
    Reload_defaultTapValue_TextCtrl();
    Reload_installedTap_ComboBox();
}
