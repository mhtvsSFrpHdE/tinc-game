#include <wx/wx.h>
#include "SettingsFrame.h"
#include <wx/spinctrl.h>
#include <wx/stdpaths.h>
#include <wx/fileconf.h>
#include <wx/config.h>
#include "Settings_SRV.h"
#include "Layout_SRV.h"
#include "String_SRV.h"

SettingsFrame::SettingsFrame(wxFrame* parentFrame) : wxFrame(parentFrame, wxID_ANY, _("Settings"))
{
    Init_CreateControls();
    Init_Layout();
}

void SettingsFrame::Init_CreateControls()
{
    rootPanel = new wxPanel(this);
    chooseLanguage_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Language"));
    chooseLanguage_ComboBox = new wxComboBox(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
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
    chooseGameMode_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Game mode"));
    chooseGameMode_ComboBox = new wxComboBox(rootPanel, wxID_ANY);
    {
        auto getComboBoxItems = Settings_SRV::ReadArray(SettingKeys_Program::lists_gameModeGames);
        if (getComboBoxItems.success) {
            chooseGameMode_ComboBox->Set(getComboBoxItems.returnBody);
        }

        auto gameModeSettingsKey = SettingKeys_Program::settings_gameMode;
        auto gameMode = Settings_SRV::programConfig->Read(gameModeSettingsKey);
        chooseGameMode_ComboBox->SetValue(gameMode);
    }
    confirmButton = new wxButton(rootPanel, wxID_ANY, _("Confirm"));
    confirmButton->Bind(wxEVT_BUTTON, &SettingsFrame::OnConfirmButtonClick, this);
    cancelButton = new wxButton(rootPanel, wxID_ANY, _("Cancel"));
    cancelButton->Bind(wxEVT_BUTTON, &SettingsFrame::OnCancelButtonClick, this);
}

void SettingsFrame::Init_Layout()
{
    namespace ls = Layout_SRV;

    this->SetSizeHints(320, 230);

    wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
    rootPanel->SetSizer(rootSizer);
    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    rootSizer->Add(chooseLanguage_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
    rootSizer->Add(chooseLanguage_ComboBox, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    rootSizer->Add(chooseGameMode_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
    rootSizer->Add(chooseGameMode_ComboBox, 0, wxLEFT, ls::SpaceToFrameBorder);
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
    namespace ss = String_SRV;

    int languageSelectedIndex = chooseLanguage_ComboBox->GetSelection();
    auto language = chooseLanguage_ComboBox_RawData[languageSelectedIndex];
    auto gameMode = chooseGameMode_ComboBox->GetValue();
    auto validGameMode = gameMode.Find(L".exe") == gameMode.size() - 4;
    if (validGameMode == false) {
        auto hint = _("Game mode must be a valid .exe file")
            + ss::newLine + _("You may select one from the list");
        wxMessageDialog(this, hint).ShowModal();
        return;
    }

    Settings_SRV::WriteLanguage(language);
    auto gameModeSettingsKey = SettingKeys_Program::settings_gameMode;
    Settings_SRV::programConfig->Write(gameModeSettingsKey, gameMode);
    Settings_SRV::programConfig->Flush();

    Close();
}

void SettingsFrame::OnCancelButtonClick(wxCommandEvent& event)
{
    Close();
}
