#include "MainFrame.h"
#include <wx/spinctrl.h>
#include "OptimizeMtuFrame.h"
#include "SettingsFrame.h"
#include <wx/dialog.h>
#include "Layout_SRV.h"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, _("Tinc Game Mode")) {
	Init_CreateControls();
	Init_BindEventHandlers();
	Init_Layout();
}

void MainFrame::Init_CreateControls()
{
	rootPanel = new wxPanel(this);
	optimizeMtuButton = new wxButton(rootPanel, wxID_ANY, _("Optimize MTU"));
	settingsButton = new wxButton(rootPanel, wxID_ANY, _("Settings"));
}

void MainFrame::Init_BindEventHandlers()
{
	optimizeMtuButton->Bind(wxEVT_BUTTON, &MainFrame::OnOptimizeMtuButton, this);
	settingsButton->Bind(wxEVT_BUTTON, &MainFrame::OnSettingsButton, this);
}

void MainFrame::Init_Layout()
{
	namespace ls = Layout_SRV;

	wxSize minSize(280, 140);
	this->SetSizeHints(minSize);

	wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
	rootPanel->SetSizer(rootSizer);
	ls::AddSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

	wxBoxSizer* optimizeMtuSizer = new wxBoxSizer(wxHORIZONTAL);
	rootSizer->Add(optimizeMtuSizer, 1, wxEXPAND);
	optimizeMtuSizer->Add(0, 0, 0, wxLEFT, ls::SpaceToFrameBorder);
	optimizeMtuSizer->Add(optimizeMtuButton, 1);
	optimizeMtuSizer->Add(0, 0, 0, wxRIGHT, ls::SpaceToFrameBorder);
	ls::AddSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

	wxBoxSizer* settingsSizer = new wxBoxSizer(wxHORIZONTAL);
	rootSizer->Add(settingsSizer, 1, wxEXPAND);
	settingsSizer->Add(0, 0, 0, wxLEFT, ls::SpaceToFrameBorder);
	settingsSizer->Add(settingsButton, 1);
	settingsSizer->Add(0, 0, 0, wxRIGHT, ls::SpaceToFrameBorder);

	ls::AddSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

	this->SetSize(minSize);
}

void MainFrame::OnOptimizeMtuButton(wxCommandEvent& evt)
{
	auto button = (wxButton*)evt.GetEventObject();
	button->Enable(false);

	OnOptimizeMtuButton_OpenOptimizeMtuFrame();
}

void MainFrame::OnOptimizeMtuButton_OpenOptimizeMtuFrame()
{
	OptimizeMtuFrame* OptimizeMtuframe = new OptimizeMtuFrame(this);
	OptimizeMtuframe->Center();
	OptimizeMtuframe->Show();
}

void MainFrame::OnSettingsButton(wxCommandEvent& evt)
{
	if (openedFrameCount == 0)
	{
		OnSettingsButton_OpenSettingsFrame();
	}
	else {
		OnSettingsButton_OtherWindowExists();
	}
}

void MainFrame::OnSettingsButton_OpenSettingsFrame()
{
	SettingsFrame* Settingsframe = new SettingsFrame(this);
	Settingsframe->Center();
	Settingsframe->Show();
}

void MainFrame::OnSettingsButton_OtherWindowExists()
{
	wxString buttonHint = _("Close all windows before enter setting interface.");
	wxString title = _("hint");
	wxMessageDialog* dial = new wxMessageDialog(NULL,
		buttonHint, title, wxOK);
	dial->ShowModal();
}
