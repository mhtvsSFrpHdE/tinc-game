#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
	void buttonHint();
	MainFrame(const wxString& title);

	void Init_CreateControls();
	void Init_BindEventHandlers();
	void Init_Layout();

	void OnUHMTUButton(wxCommandEvent& evt);
	void OnSettingsButton(wxCommandEvent& evt);

	void ButtonNoEven(wxCommandEvent& evt);

	void OpenNewWindow();
	void OpenSettingsWindow();

	int publicInt = 0;

	wxButton* optimizeMtuButton;
	wxButton* settingsButton;
	wxPanel* panel;
};
