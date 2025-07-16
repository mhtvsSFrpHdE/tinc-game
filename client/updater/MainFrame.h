#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    wxPanel* rootPanel;
    wxStaticText* text;

    void API_SRV_PostLayout();

    void Init_CreateControls();
    void Init_Layout();
    void Init_PostLayout();
};
