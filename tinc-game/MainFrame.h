#pragma once
#include <wx/wx.h>
#include <unordered_map>
#include "TapDevice_SRV.h"
#include "Networks_SRV.h"

class MainFrame : public wxFrame
{
public:
    MainFrame();

    wxStaticText* currentNetwork_StaticText = nullptr;
    wxComboBox* currentNetwork_ComboBox = nullptr;
    std::unordered_map<int, Networks_SRV::GetNetworksResult> currentNetwork_ComboBox_RawData;

    wxStaticText* currentTap_StaticText = nullptr;
    wxComboBox* currentTap_ComboBox = nullptr;
    std::unordered_map<int, WindowsAPI_SRV::GetAdaptersAddressesResult> currentTap_ComboBox_RawData;

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
