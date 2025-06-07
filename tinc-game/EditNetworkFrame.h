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

    wxButton* confirmButton = nullptr;
    void OnConfirmButtonClick(wxCommandEvent& event);

    void Init_CreateControls();
    void Init_Layout();
};
