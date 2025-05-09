#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
	MainFrame();

	wxButton* optimizeMtuButton;
	wxButton* settingsButton;

	int openedFrameCount = 0;

private:
	wxPanel* rootPanel;

	void Init_CreateControls();
	void Init_BindEventHandlers();
	void Init_Layout();

	void OnOptimizeMtuButton(wxCommandEvent& evt);
	void OnOptimizeMtuButton_OpenOptimizeMtuFrame();

	void OnSettingsButton(wxCommandEvent& evt);
	void OnSettingsButton_OpenSettingsFrame();
	void OnSettingsButton_OtherWindowExists();
};
