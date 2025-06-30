#pragma once
#include <wx/wx.h>
#include "Networks_SRV.h"

class EditNetworkFrame : public wxFrame
{
public:
    EditNetworkFrame(wxFrame* parentFrame, Networks_SRV::GetNetworksResult* network);

private:
    wxFrame* _parentFrame;
    Networks_SRV::GetNetworksResult* _network = nullptr;

    wxWindowDisabler makeModal;

    wxPanel* rootPanel = nullptr;

    wxButton* helpButton = nullptr;
    void OnHelpButtonClick(wxCommandEvent& event);
    void OnHelpFrameCloseCallback();

    wxCheckBox* gameModeCheckBox = nullptr;
    void OnGameModeCheckBoxClick(wxCommandEvent& event);
    wxCheckBox* autoConnectOnStartCheckBox = nullptr;
    wxCheckBox* showDetailedLiveLogCheckBox = nullptr;
    wxCheckBox* setMetricCheckBox = nullptr;
    wxStaticText* portNumber_StaticText = nullptr;
    wxComboBox* portNumber_ComboBox = nullptr;

    wxButton* confirmButton = nullptr;
    void OnConfirmButtonClick(wxCommandEvent& event);
    wxButton* cancelButton = nullptr;
    void OnCancelButtonClick(wxCommandEvent& event);

    void Init_CreateControls();
    void Init_Layout();

    void OnClose(wxCloseEvent& event);
};
