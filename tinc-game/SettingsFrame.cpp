#include <wx/wx.h>
#include "SettingsFrame.h"
#include <wx/spinctrl.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/fileconf.h>
#include <wx/config.h>
#include "Settings_SRV.h"
#include "Layout_SRV.h"

SettingsFrame::SettingsFrame(MainFrame* parentFrame) : wxFrame(parentFrame, wxID_ANY, _("Settings"))
{
    Init_CreateControls();
    Init_Layout();
}

void SettingsFrame::Init_CreateControls()
{
    chooseLanguage_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Language"));

    chooseLanguage_ComboBox = new wxComboBox(rootPanel, wxID_ANY);
    {
        namespace ls = Language_SRV;

        for (auto& language : ls::languageList)
        {
            chooseLanguage_ComboBox->Append(ls::languageNameMap[language]);
        }

        int readLanguage;
        bool readSuccess = Settings_SRV::config->Read(SettingKeys::language, &readLanguage);
        if (readSuccess) {
            chooseLanguage_ComboBox->SetSelection(readLanguage);
        }
    }

    confirmButton = new wxButton(rootPanel, wxID_ANY, _("Confirm"));
    confirmButton->Bind(wxEVT_BUTTON, &SettingsFrame::OnConfirmButtonClick, this);
}

void SettingsFrame::Init_Layout()
{
    namespace ls = Layout_SRV;

    this->SetSizeHints(320, 160);

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
    navigateSizer->Add(confirmButton, 1, wxRIGHT, ls::SpaceToFrameBorder);

    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);
}

void SettingsFrame::OnConfirmButtonClick(wxCommandEvent& event)
{
    int selectedIndex = chooseLanguage_ComboBox->GetSelection();
    Settings_SRV::WriteLanguage(selectedIndex);

    Close();
}
