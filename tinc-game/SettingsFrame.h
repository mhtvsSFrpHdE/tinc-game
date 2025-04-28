#pragma once
#include <wx/wx.h>
#include "MainFrame.h"

class SettingsFrame : public wxFrame
{
public:
	SettingsFrame(MainFrame* parentFrame, wxString frameTitl);
private:
	MainFrame* _parentFrame = nullptr;

	wxWindowDisabler makeModal;

	wxPanel* panel = new wxPanel(this);

	wxComboBox* languageChoice;
	wxButton* FinishButton;

	void UI_CreateControls();
	void UI_BindEventHandlers();

	void UI_OnClose(wxCloseEvent& event);

	void UI_OnFinshButtonClick(wxCommandEvent& event);
};
