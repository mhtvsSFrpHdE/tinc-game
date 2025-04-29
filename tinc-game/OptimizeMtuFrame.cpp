#include "OptimizeMtuFrame.h"
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/wx.h>
#include <wx/combobox.h>
#include <thread>
#include <codecvt>

OptimizeMtuFrame::OptimizeMtuFrame(MainFrame* parentFrame) : wxFrame(parentFrame, wxID_ANY, _("Optimize MTU")) {
	_parentFrame = parentFrame;
	_parentFrame->openedFrameCount++;

	Init_CreateControls();
	Init_BindEventHandlers();
}

void OptimizeMtuFrame::API_UI_ReportStatus(std::wstring status)
{
	liveLog->AppendText(status);
}

void OptimizeMtuFrame::API_UI_ReportMTU_IPv4(int mtu)
{
	mtuValue_IPv4->SetLabelText(std::to_string(mtu));
	reportMtuCount_IPv4 += 1;
}

void OptimizeMtuFrame::API_UI_ReportMTU_IPv6(int mtu)
{
	mtuValue_IPv6->SetLabelText(std::to_string(mtu));
	reportMtuCount_IPv6 += 1;
}

void OptimizeMtuFrame::API_UI_EndMeasureMTU(bool success, std::wstring reason)
{
	if (reportMtuCount_IPv4 != 0 && reportMtuCount_IPv6 != 0) {
		wxMessageDialog(this, _("MTU measure success")).ShowModal();
	}
	else {
		wxMessageDialog(this, _("MTU measure fail")).ShowModal();
	}

	chooseTargetAddress_ComboBox->Enable(true);
	startButton->Enable(true);
}

void OptimizeMtuFrame::Init_CreateControls()
{
	{
		wxStaticText* chooseTargetAddress_StaticText = new wxStaticText(panel, wxID_ANY, _("Target address"));
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
		chooseTargetAddress_ComboBox = new wxComboBox(panel, wxID_ANY);
		chooseTargetAddress_ComboBox->SetPosition(wxPoint(20, 50));
		chooseTargetAddress_ComboBox->SetSize(wxSize(300, 20));
		chooseTargetAddress_ComboBox->Append(wxT("10.255.60.1"));
		chooseTargetAddress_ComboBox->SetSelection(0);
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
		wxFont font(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

		mtuValue_IPv4 = new wxStaticText(panel, wxID_ANY, mtuValue_DefaultText);
		mtuValue_IPv4->SetPosition(wxPoint(80, 360));
		mtuValue_IPv4->SetSize(wxSize(50, 40));
		mtuValue_IPv4->SetFont(font);

		mtuValue_IPv6 = new wxStaticText(panel, wxID_ANY, mtuValue_DefaultText);
		mtuValue_IPv6->SetPosition(wxPoint(80, 380));
		mtuValue_IPv6->SetSize(wxSize(50, 40));
		mtuValue_IPv6->SetFont(font);
	}

	{
		wxString closeButtonText = _("Close");
		closeButton = new wxButton(panel, wxID_ANY);
		closeButton->SetLabel(closeButtonText);
		closeButton->SetPosition(wxPoint(480, 400));
		closeButton->SetSize(wxSize(100, 25));
	}
}

void OptimizeMtuFrame::Init_BindEventHandlers()
{
	Bind(wxEVT_CLOSE_WINDOW, &OptimizeMtuFrame::OnClose, this);
	startButton->Bind(wxEVT_BUTTON, &OptimizeMtuFrame::OnStartButtonClick, this);
	closeButton->Bind(wxEVT_BUTTON, &OptimizeMtuFrame::OnCloseButtonClick, this);
}

void OptimizeMtuFrame::OnClose(wxCloseEvent& event)
{
	_parentFrame->optimizeMtuButton->Enable(true);
	_parentFrame->openedFrameCount--;
	event.Skip();
}

void OptimizeMtuFrame::OnStartButtonClick(wxCommandEvent& event)
{
	startButton->Enable(false);
	chooseTargetAddress_ComboBox->Enable(false);
	wxString inputText = chooseTargetAddress_ComboBox->GetValue();
	std::wstring inputText1 = inputText.ToStdWstring();
	if (API_SRV_CheckAddressFormat(inputText1).success) {
		reportMtuCount_IPv4 = 0;
		reportMtuCount_IPv6 = 0;
		mtuValue_IPv4->SetLabelText(mtuValue_DefaultText);
		mtuValue_IPv6->SetLabelText(mtuValue_DefaultText);
		std::thread t1(&OptimizeMtuFrame::API_SRV_StartMeasureMTU, this, inputText1);
		t1.detach();
	}
	else {
		wxMessageDialog(this, _("Invalid IP address or domain")).ShowModal();
		startButton->Enable(true);
		chooseTargetAddress_ComboBox->Enable(true);
	}
}

void OptimizeMtuFrame::OnCloseButtonClick(wxCommandEvent& event)
{
	Close();
}
