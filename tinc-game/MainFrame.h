#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
    MainFrame();

    wxButton* optimizeMtu_Button;
    wxButton* manageTapDevice_Button;
    wxButton* settings_Button;

    int openedFrameCount = 0;

private:
    wxPanel* rootPanel;

    void Init_CreateControls();
    void Init_BindEventHandlers();
    void Init_Layout();

    void OnOptimizeMtuButton(wxCommandEvent& evt);
    void OnOptimizeMtuButton_OpenOptimizeMtuFrame();

    void OnManageTapButton(wxCommandEvent& evt);

    void OnSettingsButton(wxCommandEvent& evt);
    void OnSettingsButton_OpenSettingsFrame();
    void OnSettingsButton_OtherWindowExists();
};
