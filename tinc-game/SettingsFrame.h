#pragma once
#include <wx/wx.h>
#include "MainFrame.h"
#include "Language_SRV.h"

class SettingsFrame : public wxFrame
{
public:
    SettingsFrame(MainFrame* parentFrame);
private:
    wxWindowDisabler makeModal;

    wxPanel* rootPanel = new wxPanel(this);

    wxStaticText* chooseLanguage_StaticText = nullptr;
    wxComboBox* chooseLanguage_ComboBox = nullptr;
    std::unordered_map<int, Language_SRV::KnownLanguage> chooseLanguage_ComboBox_RawData;
    wxButton* confirmButton = nullptr;

    void Init_CreateControls();
    void Init_Layout();

    void OnConfirmButtonClick(wxCommandEvent& event);
};
