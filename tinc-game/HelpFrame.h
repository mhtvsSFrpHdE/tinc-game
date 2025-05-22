#pragma once
#include <wx/wx.h>
#include "OptimizeMtuFrame.h"

class HelpFrame : public wxFrame
{
public:
    HelpFrame(wxFrame* parentFrame, wxString title, std::function<void()> onCloseCallback);
    void SetHelpText(wxString helpText);

private:
    wxPanel* rootPanel = nullptr;

    wxTextCtrl* helpText_TextCtrl = nullptr;

    void Init_CreateControls();
    void Init_Layout();

    void OnClose(wxCloseEvent& event);
    std::function<void()> _onCloseCallback;
};
