#include "ApplyMtuFrame.h"

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

	wxString successText = _("Apply MTU success.");
	wxString failedText = _("Failed to apply MTU values.") + "\n";

	if (applyMtu.success) {
		wxMessageDialog(this, successText).ShowModal();
	}
	else {
		switch (applyMtu.returnBody.messageEnum) {
		case ApplyMtuResult::ApplyMtu_InvalidAdapterName:
			failedText = failedText + _("Invalid adapter name: ") + "\"" + stringSelection + "\"";
			wxMessageDialog(this, failedText).ShowModal();
			break;
		case ApplyMtuResult::ApplyMtu_Failed_IPv6:
			// Ignore IPv6 error
			wxMessageDialog(this, successText).ShowModal();
			break;
		case ApplyMtuResult::ApplyMtu_Other:
		case ApplyMtuResult::ApplyMtu_Failed_IPv4:
		case ApplyMtuResult::ApplyMtu_Failed_All:
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
