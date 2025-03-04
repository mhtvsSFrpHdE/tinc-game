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
	std::wstring wideString = L"优化MTU";

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
	UHMTUButton->SetToolTip("Please do not click repeatedly!");
}

// open new window event
void MainFrame::OpenNewWindow()
{
	UHMTU();//open new window

}



bool MainFrame::UHMTU()
{
	UHMTUFrame* UHMTUframe = new UHMTUFrame(UHMTUButton);
	UHMTUframe->SetClientSize(800, 600);
	UHMTUframe->Center();
	UHMTUframe->Show();
	return true;
	

}



