#include "UHMTUFrame.h"
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/wx.h>
#include <wx/combobox.h>
#include <thread>
#include <codecvt>

UHMTUFrame::UHMTUFrame(wxButton* parentButton) : wxFrame(nullptr, wxID_ANY, frameTitle) {
	_parentButton = parentButton;

	Bind(wxEVT_CLOSE_WINDOW, &UHMTUFrame::UI_OnClose, this);

	UI_CreateControls();
	UI_BindEventHandlers();

}

void UHMTUFrame::API_ReportStatus(std::wstring status)
{
	textCtrl->AppendText(status);
}

void UHMTUFrame::API_ReportMTU_IPv4(int mtu)
{
	nowState_IPv4->SetLabelText(std::to_string(mtu));
	attemptNumber_IPv4 += 1;
}

void UHMTUFrame::API_ReportMTU_IPv6(int mtu)
{
	nowState_IPv6->SetLabelText(std::to_string(mtu));
	attemptNumber_IPv6 += 1;
}

void UHMTUFrame::API_EndMeasureMTU(bool success, std::wstring reason)
{
	if (attemptNumber_IPv4 != 0 && attemptNumber_IPv6 != 0) {
		pass = true;
		wxMessageDialog(this, L"MTU测量成功").ShowModal();
		pass = false;
	}
	else {
		wxMessageDialog(this, L"MTU测量失败").ShowModal();
	}

	std::wstring failureCause = L"失败原因";

	if (pass == false) {
		//wxMessageDialog(this, failureCause).ShowModal();
		//textCtrl->AppendText(failureCause);
	}

	m_comboBox->Enable(true);
	beginButton->Enable(true);
}

void UHMTUFrame::UI_OnClose(wxCloseEvent& event)
{
	if (_parentButton != nullptr) {
		_parentButton->Enable(true);
	}
	event.Skip();
}

void UHMTUFrame::UI_OnComboBoxSelect(wxCommandEvent& event)
{

}

void UHMTUFrame::UI_OnStartButtonClick(wxCommandEvent& event)
{
	beginButton->Enable(false);
	m_comboBox->Enable(false);
	wxString inputText = m_comboBox->GetValue();
	std::wstring inputText1 = inputText.ToStdWstring();
	if (API_CheckAddressFormat(inputText1)) {
		attemptNumber_IPv4 = 0;
		attemptNumber_IPv6 = 0;
		nowState_IPv4->SetLabelText(DefaultState);
		nowState_IPv6->SetLabelText(DefaultState);
		std::thread t1(&UHMTUFrame::API_StartMeasureMTU, this, inputText1);
		t1.detach();
	}
	else {
		wxMessageDialog(this, "The IP entered is not a valid IP").ShowModal();
		beginButton->Enable(true);
		m_comboBox->Enable(true);
	}
}

void UHMTUFrame::UI_CreateControls()
{
	UI_staticText();
	UI_HintButton();
	UI_UserTextCtrl();
	UI_BeginButton();
	UI_SystemPointText();
	UI_StaticTextIP();
	UI_IPState();
	UI_CloseButton();
}

void UHMTUFrame::UI_staticText()
{
	wxString firstStaticText = L"选择目标地址";
	wxStaticText* staticText = new wxStaticText(panel, wxID_ANY, firstStaticText);
	staticText->SetPosition(wxPoint(20, 20));
	staticText->SetSize(wxSize(50, 40));
	wxFont font(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	staticText->SetFont(font);
}

void UHMTUFrame::UI_HintButton()
{
	wxButton* hintButton = new wxButton(panel, wxID_ANY, "?");
	hintButton->SetPosition(wxPoint(200, 22));
	hintButton->SetSize(wxSize(20, 20));
}

void UHMTUFrame::UI_UserTextCtrl()
{
	choices.Add("10.255.60.1");
	m_comboBox = new wxComboBox(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, choices, wxCB_DROPDOWN);
	m_comboBox->SetSelection(0);
	m_comboBox->SetPosition(wxPoint(20, 50));
	m_comboBox->SetSize(wxSize(300, 20));
}

void UHMTUFrame::UI_BeginButton()
{
	wxString beginButtonText = L"开始";
	beginButton = new wxButton(panel, wxID_ANY, beginButtonText);
	beginButton->SetPosition(wxPoint(350, 48));
	beginButton->SetSize(wxSize(50, 25));
}

void UHMTUFrame::UI_SystemPointText()
{
	textCtrl = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);
	textCtrl->SetPosition(wxPoint(20, 100));
	textCtrl->SetSize(wxSize(500, 250));
}

void UHMTUFrame::UI_StaticTextIP()
{
	wxString staticText4 = "IPv4:";
	wxString staticText6 = "IPv6:";


	wxStaticText* staticTextIP4 = new wxStaticText(panel, wxID_ANY, staticText4);
	wxStaticText* staticTextIP6 = new wxStaticText(panel, wxID_ANY, staticText6);

	staticTextIP4->SetPosition(wxPoint(20, 360));
	staticTextIP6->SetPosition(wxPoint(20, 380));

	staticTextIP4->SetSize(wxSize(50, 40));
	staticTextIP6->SetSize(wxSize(50, 40));

	wxFont font(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	staticTextIP4->SetFont(font);
	staticTextIP6->SetFont(font);
}

void UHMTUFrame::UI_IPState()
{
	DefaultState = L"等待中...";

	if (!Judgment) {
		nowState_IPv4 = new wxStaticText(panel, wxID_ANY, DefaultState);
		nowState_IPv6 = new wxStaticText(panel, wxID_ANY, DefaultState);

		nowState_IPv4->SetPosition(wxPoint(80, 360));
		nowState_IPv6->SetPosition(wxPoint(80, 380));

		nowState_IPv4->SetSize(wxSize(50, 40));
		nowState_IPv6->SetSize(wxSize(50, 40));

		wxFont font(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		nowState_IPv4->SetFont(font);
		nowState_IPv6->SetFont(font);
	}
	else {

	}
}

void UHMTUFrame::UI_CloseButton()
{
	wxString closeButtonText = L"关闭";
	wxButton* closeButton = new wxButton(panel, wxID_ANY, closeButtonText);
	closeButton->SetPosition(wxPoint(480, 400));
	closeButton->SetSize(wxSize(100, 25));

	//closeButton->Bind(wxEVT_BUTTON, &UHMTUFrame::UI_OnClose, this);

}

void UHMTUFrame::UI_BindEventHandlers()
{
	beginButton->Bind(wxEVT_BUTTON, &UHMTUFrame::UI_OnStartButtonClick, this);
}

const std::wstring UHMTUFrame::UHMTUFrame::frameTitle = L"优化MTU";