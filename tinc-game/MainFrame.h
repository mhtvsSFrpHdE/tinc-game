#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
    MainFrame();

    wxButton* optimizeMtu_Button = nullptr;
    wxButton* manageTapDevice_Button = nullptr;
    wxButton* integrityCheckButton = nullptr;
    wxButton* settings_Button = nullptr;

    int openedFrameCount = 0;

private:
    wxPanel* rootPanel;

    void Init_CreateControls();
    void Init_Layout();

    void OnOptimizeMtuButton(wxCommandEvent& evt);
    void OnOptimizeMtuButton_OpenOptimizeMtuFrame();
    void OnManageTapButton(wxCommandEvent& evt);
    void OnIntegrityCheckButton(wxCommandEvent& evt);
    void OnIntegrityCheckFrameCloseCallback();
    void OnSettingsButton(wxCommandEvent& evt);
    void OnSettingsButton_OpenSettingsFrame();
    void OnSettingsButton_OtherWindowExists();
};
