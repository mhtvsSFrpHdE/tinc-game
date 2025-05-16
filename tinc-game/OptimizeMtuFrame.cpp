#include "OptimizeMtuFrame.h"
#include <wx/spinctrl.h>
#include <wx/stattext.h>
#include <wx/wx.h>
#include <wx/combobox.h>
#include <thread>
#include <codecvt>
#include "ApplyMtuFrame.h"
#include "Layout_SRV.h"
#include "HelpFrame.h"
#include <sstream>
#include "String_SRV.h"
#include "Settings_SRV.h"

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

void OptimizeMtuFrame::OpenApplyMtuFrame() {
	if (reportMtuCount_IPv4 != 0 && reportMtuCount_IPv6 != 0) {
		ApplyMtuFrame* applyMtuFrame = new ApplyMtuFrame(this, mtuValue_IPv4, mtuValue_IPv6);
		applyMtuFrame->Center();
		applyMtuFrame->Show();

		applyButton->Enable(true);
	}
	else {
		wxMessageDialog(this, _("MTU measure fail")).ShowModal();
	}
}

void OptimizeMtuFrame::API_UI_EndMeasureMTU(bool success, std::wstring reason)
{
	allowCloseFrame = true;

	OpenApplyMtuFrame();

	chooseAddress_ComboBox->Enable(true);
	startButton->Enable(true);
	closeButton->Enable(true);
}

void OptimizeMtuFrame::Init_CreateControls()
{
	rootPanel = new wxPanel(this);
	chooseAddress_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Type in target address"));
	helpButton = new wxButton(rootPanel, wxID_ANY, _("Help"));

	chooseAddress_ComboBox = new wxComboBox(rootPanel, wxID_ANY);
	{
		auto getComboBoxItems = Settings_SRV::ReadArray(SettingKeys::mtuTestIp);
		if (getComboBoxItems.success) {
			chooseAddress_ComboBox->Set(getComboBoxItems.returnBody);
			chooseAddress_ComboBox->SetSelection(0);
		}
	}

	startButton = new wxButton(rootPanel, wxID_ANY, _("Start"));
	applyButton = new wxButton(rootPanel, wxID_ANY, _("Apply"));
	applyButton->Enable(false);
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
	helpButton->Bind(wxEVT_BUTTON, &OptimizeMtuFrame::OnHelpButtonClick, this);
	startButton->Bind(wxEVT_BUTTON, &OptimizeMtuFrame::OnStartButtonClick, this);
	applyButton->Bind(wxEVT_BUTTON, &OptimizeMtuFrame::OnApplyButtonClick, this);
	closeButton->Bind(wxEVT_BUTTON, &OptimizeMtuFrame::OnCloseButtonClick, this);
}

void OptimizeMtuFrame::Init_Layout()
{
	namespace ls = Layout_SRV;

	this->SetSizeHints(640, 480);

	wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
	rootPanel->SetSizer(rootSizer);
	ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

	wxBoxSizer* chooseAddressTitleSizer = new wxBoxSizer(wxHORIZONTAL);
	rootSizer->Add(chooseAddressTitleSizer);
	chooseAddressTitleSizer->Add(chooseAddress_StaticText, 1, wxLEFT, ls::SpaceToFrameBorder);
	chooseAddressTitleSizer->Add(0, 0, ls::TakeAllSpace);
	chooseAddressTitleSizer->Add(helpButton, 1, wxRIGHT, ls::SpaceToFrameBorder);
	ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

	wxBoxSizer* chooseAddressComboBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	rootSizer->Add(chooseAddressComboBoxSizer);
	chooseAddressComboBoxSizer->Add(chooseAddress_ComboBox, 3, wxLEFT, ls::SpaceToFrameBorder);
	chooseAddressComboBoxSizer->Add(startButton, 1, wxLEFT, ls::SpaceBetweenControl);
	chooseAddressComboBoxSizer->Add(applyButton, 1, wxLEFT, ls::SpaceBetweenControl);
	ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

	wxBoxSizer* liveLogSizer = new wxBoxSizer(wxHORIZONTAL);
	rootSizer->Add(liveLogSizer, 1, wxEXPAND);
	liveLogSizer->Add(0, 0, 0, wxLEFT, ls::SpaceToFrameBorder);
	liveLogSizer->Add(liveLog, 1, wxEXPAND);
	liveLogSizer->Add(0, 0, 0, wxRIGHT, ls::SpaceToFrameBorder);
	ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

	wxBoxSizer* mtuSizer_IPv4 = new wxBoxSizer(wxHORIZONTAL);
	rootSizer->Add(mtuSizer_IPv4);
	mtuSizer_IPv4->Add(mtu_IPv4_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
	mtuSizer_IPv4->Add(mtuValue_IPv4_StaticText, 0, wxLEFT, 5);
	ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

	wxBoxSizer* mtuSizer_IPv6 = new wxBoxSizer(wxHORIZONTAL);
	rootSizer->Add(mtuSizer_IPv6);
	mtuSizer_IPv6->Add(mtu_IPv6_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
	mtuSizer_IPv6->Add(mtuValue_IPv6_StaticText, 0, wxLEFT, 5);
	ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

	wxBoxSizer* navigateSizer = new wxBoxSizer(wxHORIZONTAL);
	rootSizer->Add(navigateSizer);
	navigateSizer->AddStretchSpacer(ls::TakeAllSpace);
	navigateSizer->Add(closeButton, 1, wxRIGHT, ls::SpaceToFrameBorder);

	ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);
}

void OptimizeMtuFrame::OnClose(wxCloseEvent& event)
{
	if (allowCloseFrame == false) {
		return;
	}

	_parentFrame->optimizeMtuButton->Enable(true);
	_parentFrame->openedFrameCount--;
	event.Skip();
}

void OptimizeMtuFrame::OnHelpButtonClick(wxCommandEvent& event)
{
	namespace ss = String_SRV;

	std::ostringstream helpTextStream;
	helpTextStream
		<< _("Optimize MTU function only meaningful if there is \"Path MTU Blackhole\" or tinc can't perform IPv4 Path MTU Discovery automatically on your system")
		<< ss::newLine << ss::newLine
		<< _("On Windows, your IPv4 MTU value is measured by using IPv4 DF flag, IPv6 MTU is minus 20 from this value. There is no easy way to measure IPv6 MTU directly on Windows platform")
		<< ss::newLine << ss::newLine
		<< _("All modern network program will determe TCP and UDP MTU size for themselfs even if there is \"Path MTU Blackhole\", so it's ok to have default MTU value 1500 on Ethernet adapter")
		<< ss::newLine << ss::newLine
		<< _("In theory, path mtu discovey is not an optional feature of IPv6 protocol compare to IPv4 DF flag, either applications and internet service provider must support Path MTU Discovery in IPv6 (RFC 8201). However, poor web server maintainer may decide to block all ICMP protocols to improve their server's security, so called \"Path MTU Blackhole\". It's a pretty questionable choice because this may cause some IPv6 applications to use 1280 (RFC 2460) as their MTU when path MTU discovery can't be performed, which reduces network performance");

	HelpFrame* optimizeMtuFrame_HelpFrame = new HelpFrame(this);
	optimizeMtuFrame_HelpFrame->SetHelpText(helpTextStream.str());
	optimizeMtuFrame_HelpFrame->Center();
	optimizeMtuFrame_HelpFrame->Show();

	helpButton->Enable(false);
}

void OptimizeMtuFrame::OnStartButtonClick(wxCommandEvent& event)
{
	allowCloseFrame = false;
	startButton->Enable(false);
	applyButton->Enable(false);
	closeButton->Enable(false);
	chooseAddress_ComboBox->Enable(false);

	wxString inputText = chooseAddress_ComboBox->GetValue();
	std::wstring inputText1 = inputText.ToStdWstring();
	auto checkAddressFormat = API_SRV_CheckAddressFormat(inputText1);
	if (checkAddressFormat.success) {
		reportMtuCount_IPv4 = 0;
		reportMtuCount_IPv6 = 0;
		mtuValue_IPv4_StaticText->SetLabelText(mtuValue_DefaultText);
		mtuValue_IPv6_StaticText->SetLabelText(mtuValue_DefaultText);
		std::thread t1(&OptimizeMtuFrame::API_SRV_StartMeasureMTU, this, inputText1);
		t1.detach();
	}
	else {
		wxMessageDialog(this, _("Invalid IP address or domain")).ShowModal();
		allowCloseFrame = true;
		startButton->Enable(true);
		applyButton->Enable(true);
		closeButton->Enable(true);
		chooseAddress_ComboBox->Enable(true);
	}
}

void OptimizeMtuFrame::OnApplyButtonClick(wxCommandEvent& event)
{
	OpenApplyMtuFrame();
}

void OptimizeMtuFrame::OnCloseButtonClick(wxCommandEvent& event)
{
	Close();
}
