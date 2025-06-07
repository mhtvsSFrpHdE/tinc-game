#pragma once
#include <wx/wx.h>
#include "Networks_SRV.h"

class EditNetworkFrame : public wxFrame
{
public:
    EditNetworkFrame(wxFrame* parentFrame, Networks_SRV::GetNetworksResult& network);

private:
    wxWindowDisabler makeModal;

    wxPanel* rootPanel = nullptr;

    wxCheckBox* gameModeCheckBox = nullptr;
    wxCheckBox* autoConnectOnStartCheckBox = nullptr;

    wxButton* confirmButton = nullptr;
    void OnConfirmButtonClick(wxCommandEvent& event);
    wxButton* cancelButton = nullptr;
    void OnCancelButtonClick(wxCommandEvent& event);

    void Init_CreateControls();
    void Init_Layout();
};
