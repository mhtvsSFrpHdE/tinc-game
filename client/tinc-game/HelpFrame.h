#pragma once
#include <wx/wx.h>

class HelpFrame : public wxFrame
{
public:
    HelpFrame(wxFrame* parentFrame, wxString title, std::function<void()> onCloseCallback, bool defaultInit = true);
    void SetHelpText(wxString helpText);

    wxPanel* rootPanel;
    wxTextCtrl* helpText_TextCtrl;
    wxBoxSizer* rootSizer;

private:
    wxFrame* _parentFrame;

    void Init_CreateControls(bool defaultInit);
    void Init_Layout(bool defaultInit);

    void OnClose(wxCloseEvent& event);
    std::function<void()> _onCloseCallback;
};
