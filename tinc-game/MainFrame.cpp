#include "MainFrame.h"
#include <wx/spinctrl.h>
#include "UHMTUFrame.h"
#include "SettingsFrame.h"
#include <wx/dialog.h>

void MainFrame::buttonHint(wxCommandEvent& event)
{
	wxString buttonHint = _("Close all windows before enter setting interface.");
	wxString title = _("hint");
	wxMessageDialog* dial = new wxMessageDialog(NULL,
		buttonHint, title, wxOK);
	dial->ShowModal();
}

//mainframe.main
MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	CreateControls();
	BindEventHandlers();
	SetUpSizers();
}

//create button 
wxButton* MainFrame::CreateButton(wxPanel* parent, wxString buttonText, wxSize buttonSize)
{
	wxButton* button = new wxButton(parent, wxID_ANY, buttonText, wxDefaultPosition, buttonSize);
	return button;
}


//bind event
void MainFrame::BindEventHandlers()
{
	UHMTUButton->Bind(wxEVT_BUTTON, &MainFrame::OnUHMTUButton, this);
	SettingsButton->Bind(wxEVT_BUTTON, &MainFrame::OnSettingsButton, this);
	//SettingsButton->Bind(wxEVT_BUTTON, &MainFrame::ShowMessage1, this);

}

//create control
void MainFrame::CreateControls()
{
	wxFont mainFont(wxFontInfo(wxSize(0, 24)));
	panel = new wxPanel(this);

	panel->SetFont(mainFont);

	auto whatisthistype = _("Optimize MTU");
	wxString wideString = _("Optimize MTU");
	wxString setting = _("Settings");
	UHMTUButton = CreateButton(panel, wideString, wxSize(300, 100));
	SettingsButton = CreateButton(panel, setting, wxSize(100, 50));
}

//window layout
void MainFrame::SetUpSizers()
{
	wxGridSizer* gridSizer = new wxGridSizer(3, 3, wxSize(0, 0));

	wxSizerFlags flags = wxSizerFlags().Align(wxALIGN_CENTER);
	wxSizerFlags flags2 = wxSizerFlags().Align(wxALIGN_TOP | wxALIGN_LEFT);

	gridSizer->Add(SettingsButton, flags2);

	for (int i = 0; i < 2; ++i) {
		wxStaticText* placeholder = new wxStaticText(panel, wxID_ANY, "");
		gridSizer->Add(placeholder, 0, wxEXPAND);
	}

	wxStaticText* placeholder = new wxStaticText(panel, wxID_ANY, "");
	gridSizer->Add(placeholder, 0, wxEXPAND);


	gridSizer->Add(UHMTUButton, flags);

	panel->SetSizer(gridSizer);
}

//UHM button
void MainFrame::OnUHMTUButton(wxCommandEvent& evt)
{
	auto button = (wxButton*)evt.GetEventObject();
	button->Enable(false);

	OpenNewWindow();
}

void MainFrame::OnSettingsButton(wxCommandEvent& evt)
{
	if (publicInt == 0)
	{
		OpenSettingsWindow();
	}
	else {
		wxCommandEvent dummyEvent;
		buttonHint(dummyEvent);
	}

}

void MainFrame::ButtonNoEven(wxCommandEvent& evt)
{
	UHMTUButton->SetToolTip(_("Please do not click repeatedly!"));
}

// open new window event
void MainFrame::OpenNewWindow()
{
	UHMTU();//open new window

}

void MainFrame::OpenSettingsWindow()
{
	Settings();
}



bool MainFrame::UHMTU()
{
	UHMTUFrame* UHMTUframe = new UHMTUFrame(this, _("Optimize MTU"));
	UHMTUframe->SetClientSize(600, 450);
	UHMTUframe->Center();
	UHMTUframe->Show();
	long style = UHMTUframe->GetWindowStyle();
	style &= ~wxRESIZE_BORDER;
	UHMTUframe->SetWindowStyle(style);

	return true;

}

bool MainFrame::Settings()
{
	SettingsFrame* Settingsframe = new SettingsFrame(this, _("Settings"));
	Settingsframe->SetClientSize(600, 450);
	Settingsframe->Center();
	Settingsframe->Show();
	long style = Settingsframe->GetWindowStyle();
	style &= ~wxRESIZE_BORDER;
	Settingsframe->SetWindowStyle(style);

	return true;
}

/*
void MainFrame::ShowMessage1(wxCommandEvent& event)
{
	wxMessageDialog* dial = new wxMessageDialog(NULL,
		wxT("Download completed"), wxT("Info"), wxOK);
	dial->ShowModal();
}
*/