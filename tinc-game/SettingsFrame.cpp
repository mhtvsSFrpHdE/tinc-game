#include <wx/wx.h>
#include "SettingsFrame.h"
#include <wx/spinctrl.h>
#include <wx/stdpaths.h>
#include <wx/fileconf.h>
#include <wx/config.h>
#include "Settings_SRV.h"
#include "Layout_SRV.h"

SettingsFrame::SettingsFrame(wxFrame* parentFrame) : wxFrame(parentFrame, wxID_ANY, _("Settings"))
{
    Init_CreateControls();
    Init_Layout();
}

void SettingsFrame::Init_CreateControls()
{
    rootPanel = new wxPanel(this);
    chooseLanguage_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Language"));
    chooseLanguage_ComboBox = new wxComboBox(rootPanel, wxID_ANY);
    {
        namespace ls = Language_SRV;

        int mapIndex = 0;
        for (int languageIndex = 0; languageIndex < ls::languageList.size(); languageIndex++)
        {
            auto language = ls::languageList[languageIndex];
            chooseLanguage_ComboBox_RawData.insert({ mapIndex, language });
            chooseLanguage_ComboBox->Append(ls::languageNameMap[language]);
            mapIndex = mapIndex + 1;
        }

        int readLanguage;
        bool readSuccess = Settings_SRV::programConfig->Read(SettingKeys_Program::settings_language, &readLanguage);
        if (readSuccess) {
            auto language = static_cast<ls::KnownLanguage>(readLanguage);
            auto selectionIndex = ls::languageKeyMap[language];
            chooseLanguage_ComboBox->SetSelection(selectionIndex);
        }
    }
    confirmButton = new wxButton(rootPanel, wxID_ANY, _("Confirm"));
    confirmButton->Bind(wxEVT_BUTTON, &SettingsFrame::OnConfirmButtonClick, this);
    cancelButton = new wxButton(rootPanel, wxID_ANY, _("Cancel"));
    cancelButton->Bind(wxEVT_BUTTON, &SettingsFrame::OnCancelButtonClick, this);
}

void SettingsFrame::Init_Layout()
{
    namespace ls = Layout_SRV;

    this->SetSizeHints(320, 170);

    wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
    rootPanel->SetSizer(rootSizer);
    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    rootSizer->Add(chooseLanguage_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    rootSizer->Add(chooseLanguage_ComboBox, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    rootSizer->Add(0, 0, ls::TakeAllSpace, wxEXPAND);
    wxBoxSizer* navigateSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(navigateSizer);
    navigateSizer->AddStretchSpacer(ls::TakeAllSpace);
    navigateSizer->Add(confirmButton, 1, wxRIGHT, ls::SpaceBetweenControl);
    navigateSizer->Add(cancelButton, 1, wxRIGHT, ls::SpaceToFrameBorder);

    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    this->Fit();
}

void SettingsFrame::OnConfirmButtonClick(wxCommandEvent& event)
{
    int selectedIndex = chooseLanguage_ComboBox->GetSelection();
    auto language = chooseLanguage_ComboBox_RawData[selectedIndex];
    Settings_SRV::WriteLanguage(language);

    Close();
}

void SettingsFrame::OnCancelButtonClick(wxCommandEvent& event)
{
    Close();
}
