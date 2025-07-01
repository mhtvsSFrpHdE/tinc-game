#pragma once
#include <wx/wx.h>
#include "Networks_SRV.h"

class RenameNetworkFrame : public wxFrame
{
public:
    RenameNetworkFrame(wxFrame* parentFrame, Networks_SRV::GetNetworksResult* network, std::function<void()> onCloseCallback);

private:
    wxFrame* _parentFrame;
    Networks_SRV::GetNetworksResult* _network;
    std::function<void()> _onCloseCallback;

    wxWindowDisabler makeModal;

    void Init_CreateControls();
    void Init_Layout();

    wxPanel* rootPanel;

    wxStaticText* newName_StaticText;
    wxComboBox* newName_ComboBox;

    wxButton* confirmButton;
    void OnConfirmButtonClick(wxCommandEvent& event);
    wxButton* cancelButton;
    void OnCancelButtonClick(wxCommandEvent& event);

    void OnClose(wxCloseEvent& event);
};
