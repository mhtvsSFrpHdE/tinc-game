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

    wxPanel* rootPanel = nullptr;

    wxStaticText* chooseLanguage_StaticText = nullptr;
    wxComboBox* chooseLanguage_ComboBox = nullptr;
    std::unordered_map<int, Language_SRV::KnownLanguage> chooseLanguage_ComboBox_RawData;

    wxStaticText* chooseGameMode_StaticText = nullptr;
    wxComboBox* chooseGameMode_ComboBox = nullptr;

    wxButton* confirmButton = nullptr;
    wxButton* cancelButton = nullptr;

    void Init_CreateControls();
    void Init_Layout();

    void OnConfirmButtonClick(wxCommandEvent& event);
    void OnCancelButtonClick(wxCommandEvent& event);

    void OnClose(wxCloseEvent& event);
};
