#include "OptimizeMtuFrame.h"
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/wx.h>
#include <wx/combobox.h>
#include <thread>
#include <codecvt>
#include "ApplyMtuFrame.h"
#include "Layout_SRV.h"

OptimizeMtuFrame::OptimizeMtuFrame(MainFrame* parentFrame) : wxFrame(parentFrame, wxID_ANY, _("Optimize MTU")) {
	_parentFrame = parentFrame;
	_parentFrame->openedFrameCount++;

	Init_CreateControls();
	Init_BindEventHandlers();
	Init_Layout();
}

void OptimizeMtuFrame::API_UI_ReportStatus(std::wstring status)
{
	liveLog->AppendText(status);
}

void OptimizeMtuFrame::API_UI_ReportMTU_IPv4(int mtu)
{
	mtuValue_IPv4 = mtu;
	mtuValue_IPv4_StaticText->SetLabel(std::to_string(mtu));
	reportMtuCount_IPv4 += 1;
}

void OptimizeMtuFrame::API_UI_ReportMTU_IPv6(int mtu)
{
	mtuValue_IPv6 = mtu;
	mtuValue_IPv6_StaticText->SetLabel(std::to_string(mtu));
	reportMtuCount_IPv6 += 1;
}

void OptimizeMtuFrame::API_UI_EndMeasureMTU(bool success, std::wstring reason)
{
	if (reportMtuCount_IPv4 != 0 && reportMtuCount_IPv6 != 0) {
		ApplyMtuFrame* applyMtuFrame = new ApplyMtuFrame(this, mtuValue_IPv4, mtuValue_IPv6);
		applyMtuFrame->Center();
		applyMtuFrame->Show();
	}
	else {
		wxMessageDialog(this, _("MTU measure fail")).ShowModal();
	}

	chooseAddress_ComboBox->Enable(true);
	startButton->Enable(true);
}

void OptimizeMtuFrame::Init_CreateControls()
{
	rootPanel = new wxPanel(this);
	chooseAddress_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Target address"));
	helpButton = new wxButton(rootPanel, wxID_ANY, _("Help"));

	{
		chooseAddress_ComboBox = new wxComboBox(rootPanel, wxID_ANY);
		chooseAddress_ComboBox->Append(wxT("10.255.60.1"));
		chooseAddress_ComboBox->SetSelection(0);
	}

	startButton = new wxButton(rootPanel, wxID_ANY, _("Start"));
	liveLog = new wxTextCtrl(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);
	mtu_IPv4_StaticText = new wxStaticText(rootPanel, wxID_ANY, wxT("IPv4:"));
	mtuValue_IPv4_StaticText = new wxStaticText(rootPanel, wxID_ANY, mtuValue_DefaultText);
	mtu_IPv6_StaticText = new wxStaticText(rootPanel, wxID_ANY, wxT("IPv6:"));
	mtuValue_IPv6_StaticText = new wxStaticText(rootPanel, wxID_ANY, mtuValue_DefaultText);
	closeButton = new wxButton(rootPanel, wxID_ANY, _("Close"));
}

void OptimizeMtuFrame::Init_BindEventHandlers()
{
	Bind(wxEVT_CLOSE_WINDOW, &OptimizeMtuFrame::OnClose, this);
	startButton->Bind(wxEVT_BUTTON, &OptimizeMtuFrame::OnStartButtonClick, this);
	closeButton->Bind(wxEVT_BUTTON, &OptimizeMtuFrame::OnCloseButtonClick, this);
}

void OptimizeMtuFrame::Init_Layout()
{
	namespace ls = Layout_SRV;

	this->SetSizeHints(640, 480);

	wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
	rootPanel->SetSizer(rootSizer);
	ls::AddSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

	wxBoxSizer* chooseAddressTitleSizer = new wxBoxSizer(wxHORIZONTAL);
	rootSizer->Add(chooseAddressTitleSizer);
	chooseAddressTitleSizer->Add(chooseAddress_StaticText, 1, wxLEFT, ls::SpaceToFrameBorder);
	chooseAddressTitleSizer->Add(0, 0, ls::TakeAllSpace);
	chooseAddressTitleSizer->Add(helpButton, 1, wxRIGHT, ls::SpaceToFrameBorder);
	ls::AddSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

	wxBoxSizer* chooseAddressComboBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	rootSizer->Add(chooseAddressComboBoxSizer);
	chooseAddressComboBoxSizer->Add(chooseAddress_ComboBox, 3, wxLEFT, ls::SpaceToFrameBorder);
	chooseAddressComboBoxSizer->Add(startButton, 1, wxLEFT, ls::SpaceToFrameBorder);
	ls::AddSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

	wxBoxSizer* liveLogSizer = new wxBoxSizer(wxHORIZONTAL);
	rootSizer->Add(liveLogSizer, 1, wxEXPAND);
	liveLogSizer->Add(0, 0, 0, wxLEFT, ls::SpaceToFrameBorder);
	liveLogSizer->Add(liveLog, 1, wxEXPAND);
	liveLogSizer->Add(0, 0, 0, wxRIGHT, ls::SpaceToFrameBorder);
	ls::AddSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

	wxBoxSizer* mtuSizer_IPv4 = new wxBoxSizer(wxHORIZONTAL);
	rootSizer->Add(mtuSizer_IPv4);
	mtuSizer_IPv4->Add(mtu_IPv4_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
	mtuSizer_IPv4->Add(mtuValue_IPv4_StaticText, 0, wxLEFT, 5);
	ls::AddSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

	wxBoxSizer* mtuSizer_IPv6 = new wxBoxSizer(wxHORIZONTAL);
	rootSizer->Add(mtuSizer_IPv6);
	mtuSizer_IPv6->Add(mtu_IPv6_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
	mtuSizer_IPv6->Add(mtuValue_IPv6_StaticText, 0, wxLEFT, 5);
	ls::AddSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

	wxBoxSizer* navigateSizer = new wxBoxSizer(wxHORIZONTAL);
	rootSizer->Add(navigateSizer);
	navigateSizer->AddStretchSpacer(ls::TakeAllSpace);
	navigateSizer->Add(closeButton, 1, wxRIGHT, ls::SpaceToFrameBorder);
	ls::AddSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
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
	chooseAddress_ComboBox->Enable(false);
	wxString inputText = chooseAddress_ComboBox->GetValue();
	std::wstring inputText1 = inputText.ToStdWstring();
	if (API_SRV_CheckAddressFormat(inputText1).success) {
		reportMtuCount_IPv4 = 0;
		reportMtuCount_IPv6 = 0;
		mtuValue_IPv4_StaticText->SetLabelText(mtuValue_DefaultText);
		mtuValue_IPv6_StaticText->SetLabelText(mtuValue_DefaultText);
		std::thread t1(&OptimizeMtuFrame::API_SRV_StartMeasureMTU, this, inputText1);
		t1.detach();
	}
	else {
		wxMessageDialog(this, _("Invalid IP address or domain")).ShowModal();
		startButton->Enable(true);
		chooseAddress_ComboBox->Enable(true);
	}
}

void OptimizeMtuFrame::OnCloseButtonClick(wxCommandEvent& event)
{
	Close();
}
