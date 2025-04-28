#include "OptimizeMtuFrame.h"
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/wx.h>
#include <wx/combobox.h>
#include <thread>
#include <codecvt>

OptimizeMtuFrame::OptimizeMtuFrame(MainFrame* parentFrame, wxString frameTitle) : wxFrame(nullptr, wxID_ANY, frameTitle) {
	_parentButton = parentFrame->optimizeMtuButton;
	_parentFrame = parentFrame;
	_parentFrame->openedFrameCount++;

	Bind(wxEVT_CLOSE_WINDOW, &OptimizeMtuFrame::UI_OnClose, this);

	UI_CreateControls();
	UI_BindEventHandlers();

}

void OptimizeMtuFrame::API_UI_ReportStatus(std::wstring status)
{
	liveLog->AppendText(status);
}

void OptimizeMtuFrame::API_UI_ReportMTU_IPv4(int mtu)
{
	mtuValue_IPv4->SetLabelText(std::to_string(mtu));
	attemptNumber_IPv4 += 1;
}

void OptimizeMtuFrame::API_UI_ReportMTU_IPv6(int mtu)
{
	mtuValue_IPv6->SetLabelText(std::to_string(mtu));
	attemptNumber_IPv6 += 1;
}

void OptimizeMtuFrame::API_UI_EndMeasureMTU(bool success, std::wstring reason)
{
	if (attemptNumber_IPv4 != 0 && attemptNumber_IPv6 != 0) {
		pass = true;
		wxMessageDialog(this, _("MTU measure success")).ShowModal();
		pass = false;
	}
	else {
		wxMessageDialog(this, _("MTU measure fail")).ShowModal();
	}

	std::wstring failureCause = L"失败原因";

	if (pass == false) {
		//wxMessageDialog(this, failureCause).ShowModal();
		//liveLog->AppendText(failureCause);
	}

	chooseTargetAddress_ComboBox->Enable(true);
	startButton->Enable(true);
}

void OptimizeMtuFrame::UI_OnClose(wxCloseEvent& event)
{
	if (_parentButton != nullptr) {
		_parentButton->Enable(true);
	}
	_parentFrame->openedFrameCount--;
	event.Skip();
}

void OptimizeMtuFrame::UI_OnComboBoxSelect(wxCommandEvent& event)
{

}

void OptimizeMtuFrame::UI_OnStartButtonClick(wxCommandEvent& event)
{
	startButton->Enable(false);
	chooseTargetAddress_ComboBox->Enable(false);
	wxString inputText = chooseTargetAddress_ComboBox->GetValue();
	std::wstring inputText1 = inputText.ToStdWstring();
	if (API_SRV_CheckAddressFormat(inputText1).success) {
		attemptNumber_IPv4 = 0;
		attemptNumber_IPv6 = 0;
		mtuValue_IPv4->SetLabelText(DefaultState);
		mtuValue_IPv6->SetLabelText(DefaultState);
		std::thread t1(&OptimizeMtuFrame::API_SRV_StartMeasureMTU, this, inputText1);
		t1.detach();
	}
	else {
		wxMessageDialog(this, _("Invalid IP address or domain")).ShowModal();
		startButton->Enable(true);
		chooseTargetAddress_ComboBox->Enable(true);
	}
}

void OptimizeMtuFrame::UI_OnCloseButtonClick(wxCommandEvent& event)
{
	Close();
}

void OptimizeMtuFrame::UI_CreateControls()
{
	{
		wxStaticText* chooseTargetAddress_StaticText = new wxStaticText(panel, wxID_ANY, _("Choose target address"));
		chooseTargetAddress_StaticText->SetPosition(wxPoint(20, 20));
		chooseTargetAddress_StaticText->SetSize(wxSize(50, 40));
		wxFont font(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		chooseTargetAddress_StaticText->SetFont(font);
	}

	{
		wxButton* helpButton = new wxButton(panel, wxID_ANY);
		helpButton->SetLabel(wxT("?"));
		helpButton->SetPosition(wxPoint(300, 22));
		helpButton->SetSize(wxSize(20, 20));
	}

	{
		choices.Add("10.255.60.1");
		m_comboBox = new wxComboBox(panel, wxID_ANY, wxEmptyString, wxPoint(20, 50), wxSize(300, 20), choices, wxCB_DROPDOWN);
		m_comboBox->SetSelection(0);
	}

	{
		startButton = new wxButton(panel, wxID_ANY);
		startButton->SetLabel(_("Start"));
		startButton->SetPosition(wxPoint(350, 48));
		startButton->SetSize(wxSize(50, 25));
	}

	{
		liveLog = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxPoint(20, 100), wxSize(500, 250), wxTE_READONLY | wxTE_MULTILINE);
	}

	{
		wxFont font(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

		wxStaticText* mtu_IPv4 = new wxStaticText(panel, wxID_ANY, wxT("IPv4:"));
		mtu_IPv4->SetPosition(wxPoint(20, 360));
		mtu_IPv4->SetSize(wxSize(50, 40));
		mtu_IPv4->SetFont(font);

		wxStaticText* mtu_IPv6 = new wxStaticText(panel, wxID_ANY, wxT("IPv6:"));
		mtu_IPv6->SetPosition(wxPoint(20, 380));
		mtu_IPv6->SetSize(wxSize(50, 40));
		mtu_IPv6->SetFont(font);
	}

	{
		if (!Judgment) {
			wxFont font(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

			mtuValue_IPv4 = new wxStaticText(panel, wxID_ANY, DefaultState);
			mtuValue_IPv4->SetPosition(wxPoint(80, 360));
			mtuValue_IPv4->SetSize(wxSize(50, 40));
			mtuValue_IPv4->SetFont(font);

			mtuValue_IPv6 = new wxStaticText(panel, wxID_ANY, DefaultState);
			mtuValue_IPv6->SetPosition(wxPoint(80, 380));
			mtuValue_IPv6->SetSize(wxSize(50, 40));
			mtuValue_IPv6->SetFont(font);
		}
	}

	{
		wxString closeButtonText = _("Close");
		closeButton = new wxButton(panel, wxID_ANY);
		closeButton->SetLabel(closeButtonText);
		closeButton->SetPosition(wxPoint(480, 400));
		closeButton->SetSize(wxSize(100, 25));
	}
}

void OptimizeMtuFrame::UI_BindEventHandlers()
{
	startButton->Bind(wxEVT_BUTTON, &OptimizeMtuFrame::UI_OnStartButtonClick, this);
	closeButton->Bind(wxEVT_BUTTON, &OptimizeMtuFrame::UI_OnCloseButtonClick, this);
}
