#include <boost/process.hpp>
#include <boost/process/windows.hpp>
#include "ManageTapFrame.h"
#include "TapDevice_SRV.h"
#include "..\resource\Layout_SRV.h"
#include "Style_SRV.h"
#include "..\resource\String_SRV.h"
#include <sstream>
#include "HelpFrame.h"
#include "..\resource\Resource_SRV_Bat.h"

ManageTapFrame::ManageTapFrame(wxFrame* parentFrame, std::function<void()> onCloseCallback) : wxFrame(parentFrame, wxID_ANY, _("Manage virtual network adapter"))
{
    _parentFrame = parentFrame;
    _onCloseCallback = onCloseCallback;

    TapDevice_SRV::ReloadAdapterList();

    Init_CreateControls();
    Init_Layout();
}

void ManageTapFrame::Init_CreateControls()
{
    rootPanel = new wxPanel(this);
    helpButton = new wxButton(rootPanel, wxID_ANY, _("Help"));
    helpButton->Bind(wxEVT_BUTTON, &ManageTapFrame::OnHelpButtonClick, this);
    firstTap_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Detected virtual network adapter"));
    firstTapValue_TextCtrl = new wxTextCtrl(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    Reload_firstTapValue_TextCtrl();
    manageTap_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Manage virtual network adapter"));
    installTap_Button = new wxButton(rootPanel, wxID_ANY, _("Install new"));
    installTap_Button->Bind(wxEVT_BUTTON, &ManageTapFrame::OnInstallTapButtonClick, this);
    setMetric_Button = new wxButton(rootPanel, wxID_ANY, _("Set metric"));
    setMetric_Button->Enable(false);
    setMetric_Button->Bind(wxEVT_BUTTON, &ManageTapFrame::OnSetMetricButtonclick, this);
    uninstallTapButton = new wxButton(rootPanel, wxID_ANY, _("Uninstall"));
    uninstallTapButton->Enable(false);
    uninstallTapButton->Bind(wxEVT_BUTTON, &ManageTapFrame::OnUninstallTapButtonClick, this);
    installedTap_ComboBox = new wxComboBox(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
    Reload_installedTap_ComboBox();
    installedTap_ComboBox->Bind(wxEVT_COMBOBOX, &ManageTapFrame::OnInstalledTapComboBoxChange, this);
    openDeviceManagerButton = new wxButton(rootPanel, wxID_ANY, _("Open device manager"));
    openDeviceManagerButton->Bind(wxEVT_BUTTON, &ManageTapFrame::OnHelpFrameOpenDeviceManagerButtonClick, this);
    installedTapRename_Button = new wxButton(rootPanel, wxID_ANY, _("Rename"));
    installedTapRename_Button->Bind(wxEVT_BUTTON, &ManageTapFrame::OnInstalledTapRenameButtonClick, this);
    installedTapRefresh_Button = new wxButton(rootPanel, wxID_ANY, _("Refresh"));
    installedTapRefresh_Button->Bind(wxEVT_BUTTON, &ManageTapFrame::OnInstalledTapRefreshButtonClick, this);
    closeButton = new wxButton(rootPanel, wxID_ANY, _("Close"));
    closeButton->Bind(wxEVT_BUTTON, &ManageTapFrame::OnCloseButtonClick, this);

    Bind(wxEVT_CLOSE_WINDOW, &ManageTapFrame::OnClose, this);
}

void ManageTapFrame::Init_Layout()
{
    namespace ls = Layout_SRV;

    SetSizeHints(480, 310);

    wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
    rootPanel->SetSizer(rootSizer);
    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    wxBoxSizer* helpSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(helpSizer);
    ls::AddFixedSpacer(wxLEFT, ls::SpaceToFrameBorder, helpSizer);
    helpSizer->Add(firstTap_StaticText, ls::TakeAllSpace, wxRIGHT, ls::SpaceBetweenControl);
    helpSizer->Add(helpButton, 1, wxRIGHT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    rootSizer->Add(firstTapValue_TextCtrl, 0, wxLEFT, ls::SpaceToFrameBorder);
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
    manageTapSizer->Add(setMetric_Button, 0, wxLEFT, ls::SpaceBetweenControl);
    manageTapSizer->Add(uninstallTapButton, 0, wxLEFT, ls::SpaceBetweenControl);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    wxBoxSizer* deviceManagerSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(deviceManagerSizer);
    deviceManagerSizer->Add(installedTapRename_Button, 0, wxLEFT, ls::SpaceToFrameBorder);
    deviceManagerSizer->Add(openDeviceManagerButton, 0, wxLEFT, ls::SpaceBetweenControl);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    rootSizer->AddStretchSpacer(ls::TakeAllSpace);

    wxBoxSizer* navigateSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(navigateSizer);
    navigateSizer->Add(0, 0, ls::TakeAllSpace);
    navigateSizer->Add(closeButton, 1, wxRIGHT, ls::SpaceToFrameBorder);

    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    Fit();
}

void ManageTapFrame::OnClose(wxCloseEvent& event)
{
    _onCloseCallback();
    _parentFrame->Raise();

    event.Skip();
}

void OnHelpButtonClick_InitHelpFrame(HelpFrame* helpFrame, ManageTapFrame* parentFrame) {

    // CreateControls
    helpFrame->helpText_TextCtrl = new wxTextCtrl(helpFrame->rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);
    wxButton* openDeviceManagerButton = new wxButton(helpFrame->rootPanel, wxID_ANY, _("Open device manager"));
    openDeviceManagerButton->Bind(wxEVT_BUTTON, &ManageTapFrame::OnHelpFrameOpenDeviceManagerButtonClick, parentFrame);

    // Layout
    namespace ls = Layout_SRV;

    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, helpFrame->rootSizer);

    helpFrame->rootSizer->Add(openDeviceManagerButton, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, helpFrame->rootSizer);

    wxBoxSizer* helpTextSizer = new wxBoxSizer(wxHORIZONTAL);
    helpFrame->rootSizer->Add(helpTextSizer, 1, wxEXPAND);
    helpTextSizer->Add(0, 0, 0, wxLEFT, ls::SpaceToFrameBorder);
    helpTextSizer->Add(helpFrame->helpText_TextCtrl, 1, wxEXPAND);
    helpTextSizer->Add(0, 0, 0, wxRIGHT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, helpFrame->rootSizer);

    helpFrame->rootSizer->Layout();
}

void ManageTapFrame::OnHelpButtonClick_Internal() {
    namespace ss = String_SRV;

    std::ostringstream helpTextStream;
    helpTextStream
        << _("If you don't want to install driver, don't click on \"Install new\" button")
        << ss::newLine
        << ss::newLine
        << _("Although you can deny it, but device will still add to system, and show as \"Unknown device\"")
        << ss::newLine
        << _("- If you still want to install driver, click on \"Install new\" again and agree, then you will have two virtual network adapter, one might be redundant and need to be uninstalled manually")
        << ss::newLine
        << _("- If you denied driver installation and don't want to install driver, go to Device Manager - Network adapters, right click on \"Unknown device\" and choose \"Uninstall device\"")
        << ss::newLine
        << ss::newLine
        << _("Installed virtual adapter may not be detected immediately")
        << ss::newLine
        << _("- Try click on \"Refresh\" button until textbox shows virtual network adapter name with green background")
        << ss::newLine
        << ss::newLine
        << _("Uninstall button is not uninstall driver button")
        << ss::newLine
        << _("- If want to uninstall driver, uninstall virtual network adapter until there is only one remains, go to Device manager - Network adapters, right click on \"TAP - Win32 Adapter V9\" and choose \"Uninstall device\"")
        << ss::newLine
        << _("- Check \"Attempt to remove the driver for this device\", and hit uninstall")
        << ss::newLine
        << _("- If you accidentally uninstalled all virtual network adapter, click on \"Install new\" to add one, in order to uninstall driver in device manager")
        << ss::newLine
        << ss::newLine
        << _("Tinc game can not identify which virtual network adapter is installed by Tinc game, or installed by other program, thus you must take control of the uninstall process");

    std::function<void()> redirectCallback = std::bind(&ManageTapFrame::OnHelpFrameCloseCallback, this);
    HelpFrame* manageTapFrame_HelpFrame = new HelpFrame(this, _("About virtual network adapter"), redirectCallback, false);
    OnHelpButtonClick_InitHelpFrame(manageTapFrame_HelpFrame, this);
    manageTapFrame_HelpFrame->SetHelpText(helpTextStream.str());
    manageTapFrame_HelpFrame->Center();
    manageTapFrame_HelpFrame->Show();

    helpButton->Enable(false);
}

void ManageTapFrame::OnHelpButtonClick(wxCommandEvent& event)
{
    OnHelpButtonClick_Internal();
}

void ManageTapFrame::OnInstalledTapComboBoxChange(wxCommandEvent& evt)
{
    auto hasValue = installedTap_ComboBox->GetSelection() >= 0;
    uninstallTapButton->Enable(hasValue);
    setMetric_Button->Enable(hasValue);
}

void ManageTapFrame::OnHelpFrameCloseCallback()
{
    helpButton->Enable(true);
}

void ManageTapFrame::OnInstalledTapRenameButtonClick(wxCommandEvent& evt)
{
    wxMessageDialog(this, _("Select network adapter in opened window, press F2 to rename\nClick on Refresh button after you are done")).ShowModal();

    auto openNetworkControlPanel = TapDevice_SRV::API_SRV_OpenNetworkControlPanel();
    if (openNetworkControlPanel == false) {
        wxMessageDialog(this, TapDevice_SRV::openNetworkControlPanelFailedMessage).ShowModal();
    }
}

void ManageTapFrame::OnHelpFrameOpenDeviceManagerButtonClick(wxCommandEvent& evt)
{
    auto openDeviceManager = TapDevice_SRV::API_SRV_OpenDeviceManager();
    if (openDeviceManager == false) {
        wxMessageDialog(this, TapDevice_SRV::openDeviceManagerFailedMessage).ShowModal();
    }
}

void ManageTapFrame::OnInstalledTapRefreshButtonClick(wxCommandEvent& evt)
{
    Reload();
}

void ManageTapFrame::OnSetMetricButtonclick(wxCommandEvent& evt)
{
    setMetric_Button->Enable(false);

    namespace bp = boost::process;
    namespace rsb = Resource_SRV::Bat;

    auto selectedIndex = installedTap_ComboBox->GetSelection();
    auto selectedRawData = installedTap_ComboBox_RawData[selectedIndex];
    auto friendlyName = selectedRawData.friendlyName;

    bp::system(bp::shell(), bp::args({ rsb::cmdRumCommand, rsb::netsh, L"interface", rsb::netshArgV4, L"set", L"interface",friendlyName, L"metric=1" ,L"store=persistent" }), bp::windows::hide);
    bp::system(bp::shell(), bp::args({ rsb::cmdRumCommand, rsb::netsh, L"interface", rsb::netshArgV6, L"set", L"interface", friendlyName, L"metric=1"  ,L"store=persistent" }), bp::windows::hide);

    wxMessageDialog(this, _("Metric has set")).ShowModal();

    setMetric_Button->Enable(true);
}

void ManageTapFrame::OnInstallTapButtonClick(wxCommandEvent& evt)
{
    installTap_Button->Enable(false);

    if (hasFirstTap) {
        auto askResult = wxMessageBox(_("Detected exist virtual network adapter") + String_SRV::newLine + _("Install another one?"), wxEmptyString, wxYES_NO, this);
        if (askResult == wxNO)
        {
            installTap_Button->Enable(true);
            return;
        }
    }

    if (suggestReadHelp) {
        wxMessageDialog(this, _("Suggest read help before install first virtual network adapter")).ShowModal();
        OnHelpButtonClick_Internal();
        suggestReadHelp = false;
        installTap_Button->Enable(true);
        return;
    }

    namespace ss = String_SRV;
    auto installTap = API_SRV_InstallTap();
    if (installTap.success) {
        wxMessageDialog(this, _("Successfully installed new adapter")).ShowModal();
    }
    else {
        std::wostringstream failedString;
        failedString << _("Install adapter failed:") << ss::newLine;

        if (installTap.returnBody.messageEnum == InstallTapResult::Enum::DriverNotExist) {
            failedString << _("Driver files incomplete, ")
                << installTap.returnBody.messageString << ss::space
                << _("file not found");
        }
        if (installTap.returnBody.messageEnum == InstallTapResult::Enum::InstallerNotExist) {
            failedString << "installTap.bat" << ss::space
                << _("file not found");
        }
        if (installTap.returnBody.messageEnum == InstallTapResult::Enum::NoPermission) {
            failedString << _("You may need to run tinc game as admin") << ss::newLine
                << installTap.returnBody.messageString;
        }
        if (installTap.returnBody.messageEnum == InstallTapResult::Enum::UserCanceled) {
            failedString << _("Device has configured, but you cancelled driver installation dialog") << ss::newLine
                << _("In this case, you need to uninstall redundant virtual network adapter once any other install successfully") << ss::newLine
                << _("Or go to device manager and uninstall \"Unknown device\" if you don't want to have a virtual network adapter");
        }
        if (installTap.returnBody.messageEnum == InstallTapResult::Enum::Other) {
            failedString << _("Can't confirm what happened, the raw error message is") << ss::newLine
                << installTap.returnBody.messageString;
        }

        wxMessageDialog(this, failedString.str()).ShowModal();
    }

    Reload();
    installTap_Button->Enable(true);
}

void ManageTapFrame::OnUninstallTapButtonClick(wxCommandEvent& evt)
{
    uninstallTapButton->Enable(false);

    auto selectedIndex = installedTap_ComboBox->GetSelection();
    auto adapterToUninstall = installedTap_ComboBox_RawData[selectedIndex];

    bool uninstallingLastTap = TapDevice_SRV::tapList.size() == 1;
    if (uninstallingLastTap) {
        auto askResult = wxMessageBox(_("Uninstalling last virtual network adapter") + String_SRV::newLine + _("Confirm?"), wxEmptyString, wxYES_NO, this);
        if (askResult == wxNO)
        {
            uninstallTapButton->Enable(true);
            return;
        }
    }

    namespace ss = String_SRV;
    auto uninstallTap = API_SRV_UninstallTap(adapterToUninstall);
    if (uninstallTap.success) {
        wxMessageDialog(this, _("Successfully uninstalled adapter") + String_SRV::newLine + adapterToUninstall.friendlyName + " | " + adapterToUninstall.modelName).ShowModal();
    }
    else {
        std::wostringstream failedString;
        failedString << _("Uninstall adapter failed:") << ss::newLine;

        if (uninstallTap.returnBody.messageEnum == UninstallTapResult::Enum::NoAnyAdapter) {
            failedString << _("Didn't find any adapter on your system");
        }
        if (uninstallTap.returnBody.messageEnum == UninstallTapResult::Enum::NoPermission) {
            failedString << _("You need to run tinc game as admin");
        }
        if (uninstallTap.returnBody.messageEnum == UninstallTapResult::Enum::NotFound) {
            failedString << _("Target adapter doesn't exist on your system");
        }
        if (uninstallTap.returnBody.messageEnum == UninstallTapResult::Enum::UninstallerNotExist) {
            failedString << _("Driver files incomplete, ")
                << uninstallTap.returnBody.messageString << ss::space
                << _("file not found");
        }
        if (uninstallTap.returnBody.messageEnum == UninstallTapResult::Enum::Other) {
            failedString << _("Can't confirm what happened, the raw error message is") << ss::newLine
                << uninstallTap.returnBody.messageString;
        }

        wxMessageDialog(this, failedString.str()).ShowModal();
    }

    Reload();
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
        for (size_t adapterIndex = 0; adapterIndex < getNetworkAdapterList.returnBody.size(); adapterIndex++)
        {
            auto adapter = getNetworkAdapterList.returnBody[adapterIndex];
            bool isNotLoopback = adapter.IsLoopback() == false;
            bool isTap = adapter.IsTap();
            if (isNotLoopback && isTap) {
                installedTap_ComboBox_RawData.insert({ mapIndex, adapter });
                installedTap_ComboBox->Append(adapter.friendlyName + " | " + adapter.modelName);
                mapIndex = mapIndex + 1;
            }
        }
    }
}

void ManageTapFrame::Reload_firstTapValue_TextCtrl()
{
    auto getFirstTap = TapDevice_SRV::GetFirstTap();
    if (getFirstTap.success) {
        firstTapValue_TextCtrl->SetValue(getFirstTap.returnBody.friendlyName);
        firstTapValue_TextCtrl->SetBackgroundColour(Style_SRV::passed_green);
        hasFirstTap = true;
        suggestReadHelp = false;
    }
    else {
        firstTapValue_TextCtrl->SetValue(firstTapValue_NoneText);
        firstTapValue_TextCtrl->SetBackgroundColour(Style_SRV::denied_red);
        hasFirstTap = false;
    }
}

void ManageTapFrame::Reload()
{
    TapDevice_SRV::ReloadAdapterList();
    Reload_firstTapValue_TextCtrl();
    Reload_installedTap_ComboBox();
    uninstallTapButton->Enable(false);
}
