#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);

	void Init_CreateControls();
	void Init_BindEventHandlers();
	void Init_Layout();

	void OnOptimizeMtuButton(wxCommandEvent& evt);
	void OnSettingsButton(wxCommandEvent& evt);
	void OnSettingsButton_OtherWindowExists();

	void ButtonNoEven(wxCommandEvent& evt);

	void OpenNewWindow();
	void OpenSettingsWindow();

	int publicInt = 0;

	wxButton* optimizeMtuButton;
	wxButton* settingsButton;
	wxPanel* panel;
};
