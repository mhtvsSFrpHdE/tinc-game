#pragma once
#include <wx/wx.h>
#include "Networks_SRV.h"

class EditNetworkFrame : public wxFrame
{
public:
    EditNetworkFrame(wxFrame* parentFrame, Networks_SRV::GetNetworksResult* network);

private:
    wxFrame* _parentFrame;
    Networks_SRV::GetNetworksResult* _network;

    wxWindowDisabler makeModal;

    wxPanel* rootPanel;

    wxButton* helpButton;
    void OnHelpButtonClick(wxCommandEvent& event);
    void OnHelpFrameCloseCallback();

    wxCheckBox* gameModeCheckBox;
    void OnGameModeCheckBoxClick(wxCommandEvent& event);
    wxCheckBox* autoConnectOnStartCheckBox;
    wxCheckBox* showDetailedLiveLogCheckBox;
    wxCheckBox* setMetricCheckBox;
    wxStaticText* portNumber_StaticText;
    wxComboBox* portNumber_ComboBox;

    wxButton* confirmButton;
    void OnConfirmButtonClick(wxCommandEvent& event);
    wxButton* cancelButton;
    void OnCancelButtonClick(wxCommandEvent& event);

    void Init_CreateControls();
    void Init_Layout();

    void OnClose(wxCloseEvent& event);
};
