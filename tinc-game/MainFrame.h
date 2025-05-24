#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
    MainFrame();

    wxButton* optimizeMtuButton = nullptr;
    wxButton* manageTapDeviceButton = nullptr;
    wxButton* integrityCheckButton = nullptr;
    wxButton* settingsButton = nullptr;

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
