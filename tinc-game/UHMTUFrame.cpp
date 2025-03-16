#include "UHMTUFrame.h"
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/wx.h>
#include <wx/combobox.h>
#include <thread>

UHMTUFrame::UHMTUFrame(wxButton* parentButton) : wxFrame(nullptr, wxID_ANY, frameTitle) {
	_parentButton = parentButton;

	Bind(wxEVT_CLOSE_WINDOW, &UHMTUFrame::UI_OnClose, this);

	UI_CreateControls();
	UI_BindEventHandlers();

}

void UHMTUFrame::API_ReportStatus(std::wstring status)
{
	//SRV_LiveLog->AppendText(status);
}

void UHMTUFrame::API_ReportMTU_IPv4(int mtu)
{
	//SRV_LiveMtu->AppendText("IPv4: " + std::to_wstring(mtu) + "\r");
}

void UHMTUFrame::API_ReportMTU_IPv6(int mtu)
{
	//SRV_LiveMtu->AppendText("IPv6: " + std::to_wstring(mtu) + "\r");
}

void UHMTUFrame::API_EndMeasureMTU(bool success, std::wstring reason)
{
	//SRV_LiveLog->AppendText(reason);
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
	API_StartMeasureMTU(L"1.2.4.8");
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
	choices.Add("10.80.128.1");
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
	wxTextCtrl* textCtrl = new wxTextCtrl(panel, wxID_ANY);
	textCtrl->SetPosition(wxPoint(20, 100));
	textCtrl->SetSize(wxSize(500, 250));
}

void UHMTUFrame::UI_StaticTextIP()
{
	wxString staticText4 = "IP4:";
	wxString staticText6 = "IP6:";


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
	wxString DefaultState = L"等待中...";

	if (!Judgment) {
		wxStaticText* IP4NowState = new wxStaticText(panel, wxID_ANY, DefaultState);
		wxStaticText* IP6NowState = new wxStaticText(panel, wxID_ANY, DefaultState);

		IP4NowState->SetPosition(wxPoint(80, 360));
		IP6NowState->SetPosition(wxPoint(80, 380));

		IP4NowState->SetSize(wxSize(50, 40));
		IP6NowState->SetSize(wxSize(50, 40));

		wxFont font(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		IP4NowState->SetFont(font);
		IP6NowState->SetFont(font);
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