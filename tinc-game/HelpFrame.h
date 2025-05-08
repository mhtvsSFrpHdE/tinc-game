#pragma once
#include <wx/wx.h>
#include "OptimizeMtuFrame.h"

class HelpFrame : public wxFrame
{
public:
	HelpFrame(OptimizeMtuFrame* parentFrame);
	void SetHelpText(wxString helpText);

private:
	OptimizeMtuFrame* _parentFrame = nullptr;

	wxPanel* rootPanel = nullptr;

	wxTextCtrl* helpText_TextCtrl = nullptr;

	void Init_CreateControls();
	void Init_BindEventHandlers();
	void Init_Layout();

	void OnClose(wxCloseEvent& event);
};
