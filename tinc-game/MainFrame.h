#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
	void buttonHint(wxCommandEvent& event);
	MainFrame(const wxString& title);

	void BindEventHandlers();
	void CreateControls();
	void SetUpSizers();

	void OnUHMTUButton(wxCommandEvent& evt);
	void OnSettingsButton(wxCommandEvent& evt);

	void ButtonNoEven(wxCommandEvent& evt);


	void OpenNewWindow();
	void OpenSettingsWindow();

	int publicInt = 0;

	bool Settings();

	wxButton* CreateButton(wxPanel* parent, wxString buttonText, wxSize buttonSize);
	wxButton* UHMTUButton;
	wxButton* SettingsButton;
	wxButton* languageButton;
	wxPanel* panel;
};

const int ID_INFO = 1;
const int ID_ERROR = 2;
const int ID_QUESTION = 3;
const int ID_ALERT = 4;

