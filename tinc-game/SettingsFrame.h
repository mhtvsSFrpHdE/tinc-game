#pragma once
#include <wx/wx.h>
#include "MainFrame.h"

class SettingsFrame : public wxFrame
{
public:
	SettingsFrame(MainFrame* parentFrame);
private:
	MainFrame* _parentFrame = nullptr;

	wxWindowDisabler makeModal;

	wxPanel* panel = new wxPanel(this);

	wxComboBox* chooseLanguage_ComboBox;
	wxButton* confirmButton;

	void Init_CreateControls();
	void Init_BindEventHandlers();

	void OnClose(wxCloseEvent& event);
	void OnConfirmButtonClick(wxCommandEvent& event);
};
