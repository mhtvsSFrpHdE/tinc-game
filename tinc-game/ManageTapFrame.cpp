#include "ManageTapFrame.h"
#include "TapDevice_SRV.h"
#include "Layout_SRV.h"
#include "Style_SRV.h"

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

void ManageTapFrame::Init_CreateControls()
{
    rootPanel = new wxPanel(this);
    defaultTap_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Current default virtual network adapter:"));
    defaultTapValue_TextCtrl = new wxTextCtrl(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    {
        auto getTap = TapDevice_SRV::GetDefaultTap();
        if (getTap.success) {
            defaultTapValue_TextCtrl->SetLabel(getTap.returnBody.adapter.friendlyName);
            haveDefaultTap = true;
            defaultTapValue_TextCtrl->SetBackgroundColour(Style_SRV::passed_green);
        }
        else {
            defaultTapValue_TextCtrl->SetLabel(defaultTapValue_NoneText);
        }
    }
    manageTap_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Manage virtual network adapter"));
    installTap_Button = new wxButton(rootPanel, wxID_ANY, _("Install new"));
    installedTap_ComboBox = new wxComboBox(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
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
    uninstallTapButton = new wxButton(rootPanel, wxID_ANY, _("Uninstall selected"));
    uninstallTapButton->Enable(false);
    closeButton = new wxButton(rootPanel, wxID_ANY, _("Close"));
}

void ManageTapFrame::Init_BindEventHandlers()
{
    Bind(wxEVT_CLOSE_WINDOW, &ManageTapFrame::OnClose, this);
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

void ManageTapFrame::OnCloseButtonClick(wxCommandEvent& evt)
{
    Close();
}
