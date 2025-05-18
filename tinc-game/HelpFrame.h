#pragma once
#include <wx/wx.h>
#include "OptimizeMtuFrame.h"

class HelpFrame : public wxFrame
{
public:
    HelpFrame(OptimizeMtuFrame* parentFrame, wxString title);
    void SetHelpText(wxString helpText);

private:
    OptimizeMtuFrame* _parentFrame = nullptr;

    wxPanel* rootPanel = nullptr;

    wxTextCtrl* helpText_TextCtrl = nullptr;

    void Init_CreateControls();
    void Init_Layout();

    void OnClose(wxCloseEvent& event);
};
