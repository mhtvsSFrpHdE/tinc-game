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
	textCtrl->AppendText(status);
}

void OptimizeMtuFrame::API_UI_ReportMTU_IPv4(int mtu)
{
	nowState_IPv4->SetLabelText(std::to_string(mtu));
	attemptNumber_IPv4 += 1;
}

void OptimizeMtuFrame::API_UI_ReportMTU_IPv6(int mtu)
{
	nowState_IPv6->SetLabelText(std::to_string(mtu));
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
		//textCtrl->AppendText(failureCause);
	}

	m_comboBox->Enable(true);
	beginButton->Enable(true);
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
	beginButton->Enable(false);
	m_comboBox->Enable(false);
	wxString inputText = m_comboBox->GetValue();
	std::wstring inputText1 = inputText.ToStdWstring();
	if (API_SRV_CheckAddressFormat(inputText1).success) {
		attemptNumber_IPv4 = 0;
		attemptNumber_IPv6 = 0;
		nowState_IPv4->SetLabelText(DefaultState);
		nowState_IPv6->SetLabelText(DefaultState);
		std::thread t1(&OptimizeMtuFrame::API_SRV_StartMeasureMTU, this, inputText1);
		t1.detach();
	}
	else {
		wxMessageDialog(this, _("Invalid IP address or domain")).ShowModal();
		beginButton->Enable(true);
		m_comboBox->Enable(true);
	}
}

void OptimizeMtuFrame::UI_OnCloseButtonClick(wxCommandEvent& event)
{
	Close();
}

void OptimizeMtuFrame::UI_CreateControls()
{
	{
		wxStaticText* staticText = new wxStaticText(panel, wxID_ANY, _("Choose target address"));
		staticText->SetPosition(wxPoint(20, 20));
		staticText->SetSize(wxSize(50, 40));
		wxFont font(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		staticText->SetFont(font);
	}

	{
		wxButton* hintButton = new wxButton(panel, wxID_ANY, wxT("?"));
		hintButton->SetPosition(wxPoint(300, 22));
		hintButton->SetSize(wxSize(20, 20));
	}

	{
		choices.Add("10.255.60.1");
		m_comboBox = new wxComboBox(panel, wxID_ANY, wxEmptyString, wxPoint(20, 50), wxSize(300, 20), choices, wxCB_DROPDOWN);
		m_comboBox->SetSelection(0);
	}

	{
		beginButton = new wxButton(panel, wxID_ANY, _("Start"));
		beginButton->SetPosition(wxPoint(350, 48));
		beginButton->SetSize(wxSize(50, 25));
	}

	{
		textCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxPoint(20, 100), wxSize(500, 250), wxTE_READONLY | wxTE_MULTILINE);
	}

	{
		wxFont font(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

		wxStaticText* staticTextIP4 = new wxStaticText(panel, wxID_ANY, wxT("IPv4:"));
		staticTextIP4->SetPosition(wxPoint(20, 360));
		staticTextIP4->SetSize(wxSize(50, 40));
		staticTextIP4->SetFont(font);

		wxStaticText* staticTextIP6 = new wxStaticText(panel, wxID_ANY, wxT("IPv6:"));
		staticTextIP6->SetPosition(wxPoint(20, 380));
		staticTextIP6->SetSize(wxSize(50, 40));
		staticTextIP6->SetFont(font);
	}

	{
		if (!Judgment) {
			wxFont font(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

			nowState_IPv4 = new wxStaticText(panel, wxID_ANY, DefaultState);
			nowState_IPv4->SetPosition(wxPoint(80, 360));
			nowState_IPv4->SetSize(wxSize(50, 40));
			nowState_IPv4->SetFont(font);

			nowState_IPv6 = new wxStaticText(panel, wxID_ANY, DefaultState);
			nowState_IPv6->SetPosition(wxPoint(80, 380));
			nowState_IPv6->SetSize(wxSize(50, 40));
			nowState_IPv6->SetFont(font);
		}
	}

	{
		wxString closeButtonText = _("Close");
		closeButton = new wxButton(panel, wxID_ANY, closeButtonText);
		closeButton->SetPosition(wxPoint(480, 400));
		closeButton->SetSize(wxSize(100, 25));
	}
}

void OptimizeMtuFrame::UI_BindEventHandlers()
{
	beginButton->Bind(wxEVT_BUTTON, &OptimizeMtuFrame::UI_OnStartButtonClick, this);
	closeButton->Bind(wxEVT_BUTTON, &OptimizeMtuFrame::UI_OnCloseButtonClick, this);
}
