#pragma once
#include <wx/wx.h>
#include "MainFrame.h"

class SettingsFrame : public wxFrame
{
public:
	wxPanel* panel = new wxPanel(this);
	SettingsFrame(MainFrame* parentFrame, wxString frameTitl);
private:
	wxWindowDisabler makeModal;
	wxButton* _parentButton = nullptr;
	MainFrame* _parentFrame = nullptr;
	wxComboBox* languageChoice;
	wxButton* FinishButton;

	void UI_CreateControls();
	void UI_staticText1(); 
	void UI_FinishButton();
	void UI_languageChoice();
	void UI_BindEventHandlers();

	void UI_OnClose(wxCloseEvent& event);

	void UI_OnFinshButtonClick(wxCommandEvent& event);
};


