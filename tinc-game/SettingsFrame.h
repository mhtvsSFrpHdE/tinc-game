#pragma once
#include <wx/wx.h>
#include "MainFrame.h"

class SettingsFrame : public wxFrame
{
public:
    SettingsFrame(MainFrame* parentFrame);
private:
    wxWindowDisabler makeModal;

    wxPanel* rootPanel = new wxPanel(this);

    wxStaticText* chooseLanguage_StaticText = nullptr;
    wxComboBox* chooseLanguage_ComboBox = nullptr;
    wxButton* confirmButton = nullptr;

    void Init_CreateControls();
    void Init_Layout();

    void OnConfirmButtonClick(wxCommandEvent& event);
};
