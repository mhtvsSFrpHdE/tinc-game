#pragma once
#include <wx/wx.h>
#include "Language_SRV.h"

class SettingsFrame : public wxFrame
{
public:
    SettingsFrame(wxFrame* parentFrame);
private:
    wxFrame* _parentFrame;

    wxWindowDisabler makeModal;

    wxPanel* rootPanel;

    wxStaticText* chooseLanguage_StaticText;
    wxComboBox* chooseLanguage_ComboBox;
    std::unordered_map<int, Language_SRV::KnownLanguage> chooseLanguage_ComboBox_RawData;

    wxStaticText* chooseGameMode_StaticText;
    wxComboBox* chooseGameMode_ComboBox;

    wxButton* confirmButton;
    wxButton* cancelButton;

    void Init_CreateControls();
    void Init_Layout();

    void OnConfirmButtonClick(wxCommandEvent& event);
    void OnCancelButtonClick(wxCommandEvent& event);

    void OnClose(wxCloseEvent& event);
};
