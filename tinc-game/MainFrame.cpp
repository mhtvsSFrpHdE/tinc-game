#include "MainFrame.h"
#include <wx/spinctrl.h>
#include "OptimizeMtuFrame.h"
#include "SettingsFrame.h"
#include <wx/dialog.h>

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, _("Tinc Game Mode")) {
	Init_CreateControls();
	Init_BindEventHandlers();
	Init_Layout();
}

void MainFrame::Init_CreateControls()
{
	wxFont mainFont(wxFontInfo(wxSize(0, 24)));
	panel = new wxPanel(this);
	panel->SetFont(mainFont);

	wxString wideString = _("Optimize MTU");
	optimizeMtuButton = new wxButton(panel, wxID_ANY, wideString, wxDefaultPosition, wxSize(300, 100));

	wxString setting = _("Settings");
	settingsButton = new wxButton(panel, wxID_ANY, setting, wxDefaultPosition, wxSize(100, 50));
}

void MainFrame::Init_BindEventHandlers()
{
	optimizeMtuButton->Bind(wxEVT_BUTTON, &MainFrame::OnOptimizeMtuButton, this);
	settingsButton->Bind(wxEVT_BUTTON, &MainFrame::OnSettingsButton, this);
}

void MainFrame::Init_Layout()
{
	wxGridSizer* gridSizer = new wxGridSizer(3, 3, wxSize(0, 0));

	wxSizerFlags flags = wxSizerFlags().Align(wxALIGN_CENTER);
	wxSizerFlags flags2 = wxSizerFlags().Align(wxALIGN_TOP | wxALIGN_LEFT);

	gridSizer->Add(settingsButton, flags2);

	for (int i = 0; i < 2; ++i) {
		wxStaticText* placeholder = new wxStaticText(panel, wxID_ANY, "");
		gridSizer->Add(placeholder, 0, wxEXPAND);
	}

	wxStaticText* placeholder = new wxStaticText(panel, wxID_ANY, "");
	gridSizer->Add(placeholder, 0, wxEXPAND);


	gridSizer->Add(optimizeMtuButton, flags);

	panel->SetSizer(gridSizer);
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
	OptimizeMtuframe->SetClientSize(600, 450);
	OptimizeMtuframe->Center();
	OptimizeMtuframe->Show();
	long style = OptimizeMtuframe->GetWindowStyle();
	style &= ~wxRESIZE_BORDER;
	OptimizeMtuframe->SetWindowStyle(style);
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
	SettingsFrame* Settingsframe = new SettingsFrame(this, _("Settings"));
	Settingsframe->SetClientSize(600, 450);
	Settingsframe->Center();
	Settingsframe->Show();
	long style = Settingsframe->GetWindowStyle();
	style &= ~wxRESIZE_BORDER;
	Settingsframe->SetWindowStyle(style);
}

void MainFrame::OnSettingsButton_OtherWindowExists()
{
	wxString buttonHint = _("Close all windows before enter setting interface.");
	wxString title = _("hint");
	wxMessageDialog* dial = new wxMessageDialog(NULL,
		buttonHint, title, wxOK);
	dial->ShowModal();
}
