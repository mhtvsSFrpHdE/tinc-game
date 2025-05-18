#include "ApplyMtuFrame.h"
#include "String_SRV.h"
#include "Layout_SRV.h"
#include "TapDevice_SRV.h"

ApplyMtuFrame::ApplyMtuFrame(OptimizeMtuFrame* parentFrame, int mtuValue_IPv4, int mtuValue_IPv6) : wxFrame(parentFrame, wxID_ANY, _("Apply MTU"))
{
    _mtuValue_IPv4 = mtuValue_IPv4;
    _mtuValue_IPv6 = mtuValue_IPv6;

    Init_CreateControls();
    Init_BindEventHandlers();
    Init_Layout();
}

void ApplyMtuFrame::Init_CreateControls()
{
    rootPanel = new wxPanel(this);
    chooseAdapter_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Set MTU to network interface"));

    chooseAdapter_ComboBox = new wxComboBox(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
    {
        auto getNetworkAdapterList = TapDevice_SRV::API_SRV_GetNetworkAdapterList();
        if (getNetworkAdapterList.success) {
            int mapIndex = 0;
            for (int adapterIndex = 0; adapterIndex < getNetworkAdapterList.returnBody.size(); adapterIndex++)
            {
                auto adapter = getNetworkAdapterList.returnBody[adapterIndex];
                if (adapter.isLoopback() == false) {
                    chooseAdapter_ComboBox_RawData.insert({ mapIndex, adapter });
                    chooseAdapter_ComboBox->Append(adapter.friendlyName + " | " + adapter.modelName);
                    mapIndex = mapIndex + 1;
                }
            }
        }
    }

    chooseAdapter_HelpMeDecideButton = new wxButton(rootPanel, wxID_ANY, _("Help me decide"));
    displayMtu_IPv4 = new wxStaticText(rootPanel, wxID_ANY, wxT("IPv4: ") + std::to_wstring(_mtuValue_IPv4));
    displayMtu_IPv6 = new wxStaticText(rootPanel, wxID_ANY, wxT("IPv6: ") + std::to_wstring(_mtuValue_IPv6));
    yourCommand_StaticText = new wxStaticText(rootPanel, wxID_ANY, wxT("Your command"));
    yourCommand_TextCtrl = new wxTextCtrl(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    yourCommand_CopyButton = new wxButton(rootPanel, wxID_ANY, _("Copy"));
    yourCommand_CopyButton->Enable(false);
    navigate_ApplyButton = new wxButton(rootPanel, wxID_ANY, _("Apply"));
    navigate_CancelButton = new wxButton(rootPanel, wxID_ANY, _("Cancel"));
}

void ApplyMtuFrame::Init_BindEventHandlers()
{
    chooseAdapter_HelpMeDecideButton->Bind(wxEVT_BUTTON, &ApplyMtuFrame::OnHelpMeDecideButton, this);
    chooseAdapter_ComboBox->Bind(wxEVT_COMBOBOX, &ApplyMtuFrame::OnChooseTargetInterfaceChange, this);
    yourCommand_CopyButton->Bind(wxEVT_BUTTON, &ApplyMtuFrame::OnCopyButton, this);
    navigate_ApplyButton->Bind(wxEVT_BUTTON, &ApplyMtuFrame::OnConfirmButton, this);
    navigate_CancelButton->Bind(wxEVT_BUTTON, &ApplyMtuFrame::OnCancelButton, this);
}

void ApplyMtuFrame::Init_Layout()
{
    namespace ls = Layout_SRV;

    this->SetSizeHints(320, 270);

    wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
    rootPanel->SetSizer(rootSizer);
    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    rootSizer->Add(chooseAdapter_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    wxBoxSizer* chooseAdapterSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(chooseAdapterSizer);
    ls::AddFixedSpacer(wxLEFT, ls::SpaceToFrameBorder, chooseAdapterSizer);
    chooseAdapter_ComboBox->SetMinSize(wxSize(100, -1));
    chooseAdapterSizer->Add(chooseAdapter_ComboBox, ls::TakeAllSpace, wxRIGHT, ls::SpaceBetweenControl);
    chooseAdapterSizer->Add(chooseAdapter_HelpMeDecideButton, 1, wxRIGHT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    rootSizer->Add(displayMtu_IPv4, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
    rootSizer->Add(displayMtu_IPv6, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
    rootSizer->Add(yourCommand_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    wxBoxSizer* yourCommandSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(yourCommandSizer);
    ls::AddFixedSpacer(wxLEFT, ls::SpaceToFrameBorder, yourCommandSizer);
    yourCommandSizer->Add(yourCommand_TextCtrl, ls::TakeAllSpace, wxRIGHT, ls::SpaceBetweenControl);
    yourCommandSizer->Add(yourCommand_CopyButton, 1, wxRIGHT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    wxBoxSizer* navigateSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(navigateSizer);
    navigateSizer->AddStretchSpacer(ls::TakeAllSpace);
    navigateSizer->Add(navigate_ApplyButton, 1, wxRIGHT, ls::SpaceBetweenControl);
    navigateSizer->Add(navigate_CancelButton, 1, wxRIGHT, ls::SpaceToFrameBorder);

    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);
}

void ApplyMtuFrame::OnHelpMeDecideButton(wxCommandEvent& evt)
{
    auto openNetworkControlPanel = API_SRV_OpenNetworkControlPanel();
    if (openNetworkControlPanel == false) {
        wxMessageDialog(this, _("Failed to open \"control.exe\".")).ShowModal();
    }
}

void ApplyMtuFrame::OnChooseTargetInterfaceChange(wxCommandEvent& evt)
{
    auto selectedIndex = chooseAdapter_ComboBox->GetSelection();
    auto adapter = chooseAdapter_ComboBox_RawData[selectedIndex];

    auto netshCommand = API_SRV_GetNetshCommand(adapter.friendlyName, _mtuValue_IPv4, _mtuValue_IPv6);
    yourCommand_TextCtrl->SetLabel(netshCommand);
    yourCommand_CopyButton->Enable(true);
}

void ApplyMtuFrame::OnCopyButton(wxCommandEvent& evt)
{
    auto commandText = yourCommand_TextCtrl->GetLabel();
    API_SRV_CopyNetshCommand(commandText.ToStdWstring());
    API_SRV_OpenCommandPrompt();
}

void ApplyMtuFrame::OnConfirmButton(wxCommandEvent& evt)
{
    const int NO_SELECTION = -1;
    auto selectedIndex = chooseAdapter_ComboBox->GetSelection();
    if (selectedIndex == NO_SELECTION) {
        wxMessageDialog(this, _("Select network interface before apply MTU values.")).ShowModal();
        return;
    }

    auto adapter = chooseAdapter_ComboBox_RawData[selectedIndex];
    auto applyMtu = API_SRV_ApplyMtu(_mtuValue_IPv4, _mtuValue_IPv6, adapter.friendlyName);

    namespace ss = String_SRV;
    wxString successText = _("Apply MTU success");
    wxString failedText = _("Failed to apply MTU values") + ss::newLine;

    if (applyMtu.success) {
        wxMessageDialog(this, successText).ShowModal();
        Close();
    }
    else {
        switch (applyMtu.returnBody.messageEnum) {
        case ApplyMtuResult::ApplyMtu_InvalidAdapterName:
            failedText = failedText + _("Invalid adapter name: ") + ss::doubleQuotes + adapter.friendlyName + ss::doubleQuotes + ss::newLine
                + _("If your adapter name contains non English characters, try copy command and run manually in command prompt. You can right click in command prompt window to paste your command");
            wxMessageDialog(this, failedText).ShowModal();
            break;
        case ApplyMtuResult::ApplyMtu_Failed_IPv6:
            // Ignore IPv6 error
            wxMessageDialog(this, successText).ShowModal();
            break;
        case ApplyMtuResult::ApplyMtu_Other:
        case ApplyMtuResult::ApplyMtu_Failed_IPv4:
            failedText = failedText + _("Unknown error: ") + applyMtu.returnBody.messageString;
            wxMessageDialog(this, failedText).ShowModal();
            break;
        }
    }
}

void ApplyMtuFrame::OnCancelButton(wxCommandEvent& evt)
{
    Close();
}
