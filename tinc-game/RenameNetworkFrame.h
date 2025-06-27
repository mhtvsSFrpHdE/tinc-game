#pragma once
#include <wx/wx.h>
#include "Networks_SRV.h"

class RenameNetworkFrame : public wxFrame
{
public:
    RenameNetworkFrame(wxFrame* parentFrame, Networks_SRV::GetNetworksResult* network);

private:
    Networks_SRV::GetNetworksResult* _network = nullptr;

    wxWindowDisabler makeModal;

    void Init_CreateControls();
    void Init_Layout();

    wxPanel* rootPanel = nullptr;

    wxStaticText* newName_StaticText = nullptr;
    wxComboBox* newName_ComboBox = nullptr;

    wxButton* confirmButton = nullptr;
    void OnConfirmButtonClick(wxCommandEvent& event);
    wxButton* cancelButton = nullptr;
    void OnCancelButtonClick(wxCommandEvent& event);
};
