#pragma once
#include <wx/wx.h>
#include "../resource/Lock_SRV.h"

class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    std::mutex uiMutex;
    std::condition_variable uiCb;

    wxPanel* rootPanel;
    wxStaticText* text;

    void API_SRV_PostLayout();

    void Init_CreateControls();
    void Init_Layout();
    void Init_PostLayout();
};
