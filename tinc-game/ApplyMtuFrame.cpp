#include "ApplyMtuFrame.h"
#include "String_SRV.h"

ApplyMtuFrame::ApplyMtuFrame(OptimizeMtuFrame* parentFrame, int mtuValue_IPv4, int mtuValue_IPv6) : wxFrame(parentFrame, wxID_ANY, _("Apply MTU"))
{
	_mtuValue_IPv4 = mtuValue_IPv4;
	_mtuValue_IPv6 = mtuValue_IPv6;

	Init_CreateControls();
	Init_BindEventHandlers();
}

void ApplyMtuFrame::Init_CreateControls()
{
	{
		wxStaticText* setMtuToInterface = new wxStaticText(panel, wxID_ANY, _("Set MTU to network interface"));
		setMtuToInterface->SetPosition(wxPoint(20, 20));
	}

	{
		chooseTargetInterface = new wxComboBox(panel, wxID_ANY);
		chooseTargetInterface->SetPosition(wxPoint(20, 50));
		chooseTargetInterface->SetSize(wxSize(150, 20));

		auto getNetworkAdapterList = API_SRV_GetNetworkAdapterList();
		if (getNetworkAdapterList.success) {
			for (auto& adapterName : getNetworkAdapterList.returnBody)
			{
				chooseTargetInterface->Append(adapterName);
			}
		}
	}

	{
		helpMeDecideButton = new wxButton(panel, wxID_ANY, _("Help me decide"));
		helpMeDecideButton->SetPosition(wxPoint(180, 50));
	}

	{
		wxStaticText* setMtuToInterface = new wxStaticText(panel, wxID_ANY, wxT("IPv4: ") + std::to_wstring(_mtuValue_IPv4));
		setMtuToInterface->SetPosition(wxPoint(20, 80));
	}

	{
		wxStaticText* setMtuToInterface = new wxStaticText(panel, wxID_ANY, wxT("IPv6: ") + std::to_wstring(_mtuValue_IPv6));
		setMtuToInterface->SetPosition(wxPoint(20, 110));
	}

	{
		wxStaticText* yourCommand = new wxStaticText(panel, wxID_ANY, wxT("Your command"));
		yourCommand->SetPosition(wxPoint(20, 140));
	}

	{
		yourCommand_TextCtrl = new wxTextCtrl(panel, wxID_ANY);
		yourCommand_TextCtrl->SetPosition(wxPoint(20, 170));
		yourCommand_TextCtrl->SetSize(wxSize(150, -1));
	}

	{
		copyButton = new wxButton(panel, wxID_ANY, _("Copy"));
		copyButton->SetPosition(wxPoint(180, 170));
	}

	{
		confirmButton = new wxButton(panel, wxID_ANY, _("Confirm"));
		confirmButton->SetPosition(wxPoint(140, 210));
	}

	{
		cancelButton = new wxButton(panel, wxID_ANY, _("Cancel"));
		cancelButton->SetPosition(wxPoint(230, 210));
	}
}

void ApplyMtuFrame::Init_BindEventHandlers()
{
	helpMeDecideButton->Bind(wxEVT_BUTTON, &ApplyMtuFrame::OnHelpMeDecideButton, this);
	chooseTargetInterface->Bind(wxEVT_COMBOBOX, &ApplyMtuFrame::OnChooseTargetInterfaceChange, this);
	copyButton->Bind(wxEVT_BUTTON, &ApplyMtuFrame::OnCopyButton, this);
	confirmButton->Bind(wxEVT_BUTTON, &ApplyMtuFrame::OnConfirmButton, this);
	cancelButton->Bind(wxEVT_BUTTON, &ApplyMtuFrame::OnCancelButton, this);
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
	auto selectedInterfaceName = chooseTargetInterface->GetStringSelection();
	auto netshCommand = API_SRV_GetNetshCommand(selectedInterfaceName.ToStdWstring(), _mtuValue_IPv4, _mtuValue_IPv6);
	yourCommand_TextCtrl->SetLabel(netshCommand);
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
	auto selection = chooseTargetInterface->GetSelection();
	if (selection == NO_SELECTION) {
		wxMessageDialog(this, _("Select network interface before apply MTU values.")).ShowModal();
		return;
	}

	auto stringSelection = chooseTargetInterface->GetStringSelection();
	auto applyMtu = API_SRV_ApplyMtu(_mtuValue_IPv4, _mtuValue_IPv6, stringSelection.ToStdWstring());

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
			failedText = failedText + _("Invalid adapter name: ") + ss::doubleQuotes + stringSelection + ss::doubleQuotes + ss::newLine
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
