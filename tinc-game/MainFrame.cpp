#include "MainFrame.h"
#include <wx/spinctrl.h>
#include "UHMTUFrame.h"

//mainframe.main
MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	CreateControls();
	BindEventHandlers();
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
}

//create control
void MainFrame::CreateControls()
{
	wxFont mainFont(wxFontInfo(wxSize(0, 24)));
	panel = new wxPanel(this);

	panel->SetFont(mainFont);

	auto whatisthistype = _("Optimize MTU");
	wxString wideString = _("Optimize MTU");
	UHMTUButton = CreateButton(panel, wideString, wxSize(300, 100));
}

//window layout
void MainFrame::SetUpSizers()
{
	wxGridSizer* gridSizer = new wxGridSizer(1, 1, wxSize(0, 0));
	panel->SetSizer(gridSizer);
	gridSizer->Add(UHMTUButton);

}

//UHM button
void MainFrame::OnUHMTUButton(wxCommandEvent& evt)
{
	auto button = (wxButton*)evt.GetEventObject();
	button->Enable(false);
	
	OpenNewWindow();
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



bool MainFrame::UHMTU()
{
	UHMTUFrame* UHMTUframe = new UHMTUFrame(UHMTUButton, _("Optimize MTU"));
	UHMTUframe->SetClientSize(600, 450);
	UHMTUframe->Center();
	UHMTUframe->Show();
	long style = UHMTUframe->GetWindowStyle();
	style &= ~wxRESIZE_BORDER;
	UHMTUframe->SetWindowStyle(style);
	return true;

}



