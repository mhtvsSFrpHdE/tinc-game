#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);

	void BindEventHandlers();
	void CreateControls();
	void SetUpSizers();

	void OnUHMTUButton(wxCommandEvent& evt);

	void ButtonNoEven(wxCommandEvent& evt);

	void OpenNewWindow(); 

	bool UHMTU();

	wxButton* CreateButton(wxPanel* parent, wxString buttonText, wxSize buttonSize );
	wxButton* UHMTUButton; 
	wxPanel* panel;
};
