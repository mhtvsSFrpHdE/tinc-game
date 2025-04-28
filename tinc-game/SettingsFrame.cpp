#include <wx/wx.h>
#include "SettingsFrame.h"
#include <wx/spinctrl.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/fileconf.h>
#include <wx/config.h>
#include "Settings_SRV.h"

SettingsFrame::SettingsFrame(MainFrame* parentFrame, wxString frameTitle) : wxFrame(nullptr, wxID_ANY, frameTitle)
{
	_parentFrame = parentFrame;

	Init_CreateControls();
	Init_BindEventHandlers();
}

void SettingsFrame::Init_CreateControls()
{
	{
		wxString staticText1 = _("language");
		wxStaticText* chooseLanguage_StaticText = new wxStaticText(panel, wxID_ANY, staticText1);
		chooseLanguage_StaticText->SetPosition(wxPoint(20, 20));

		chooseLanguage_StaticText->SetSize(wxSize(50, 40));

		wxFont font(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

		chooseLanguage_StaticText->SetFont(font);
	}

	{
		int value;
		Settings_SRV::config->Read(SettingKeys::language, &value);

		wxArrayString choices;
		choices.Add(_("Auto (System Default)"));
		choices.Add("English");
		choices.Add(wxT("简体中文"));

		chooseLanguage_ComboBox = new wxComboBox(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, choices, wxCB_DROPDOWN);
		chooseLanguage_ComboBox->SetPosition(wxPoint(100, 20));
		chooseLanguage_ComboBox->SetSelection(0);
		if (value == wxLANGUAGE_UNKNOWN) {
			chooseLanguage_ComboBox->SetSelection(0);
		}
		else if (value == wxLANGUAGE_ENGLISH_US) {
			chooseLanguage_ComboBox->SetSelection(1);
		}
		else if (value == wxLANGUAGE_CHINESE_SIMPLIFIED) {
			chooseLanguage_ComboBox->SetSelection(2);
		}
	}

	{
		wxString ButtonText = _("Confirm");
		confirmButton = new wxButton(panel, wxID_ANY, ButtonText);
		confirmButton->SetPosition(wxPoint(250, 400));
		confirmButton->SetSize(wxSize(100, 40));

		wxFont buttonFont = confirmButton->GetFont();
		buttonFont.SetPointSize(15);
		confirmButton->SetFont(buttonFont);
	}
}

void SettingsFrame::Init_BindEventHandlers()
{
	confirmButton->Bind(wxEVT_BUTTON, &SettingsFrame::OnConfirmButtonClick, this);
	Bind(wxEVT_CLOSE_WINDOW, &SettingsFrame::OnClose, this);
}

void SettingsFrame::OnClose(wxCloseEvent& event)
{
	_parentFrame->settingsButton->Enable(true);

	event.Skip();
}

void SettingsFrame::OnConfirmButtonClick(wxCommandEvent& event)
{
	int selectedIndex = chooseLanguage_ComboBox->GetSelection();
	Settings_SRV::WriteLanguage(selectedIndex);

	Destroy();
}
