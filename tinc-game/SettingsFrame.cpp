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
	_parentButton = parentFrame->SettingsButton;
	_parentFrame = parentFrame;

	UI_CreateControls();
	UI_BindEventHandlers();
}

void SettingsFrame::UI_CreateControls()
{
	UI_staticText1();
	UI_languageChoice();
	UI_FinishButton();
}

void SettingsFrame::UI_staticText1()
{
	wxString staticText1 = _("language");
	wxStaticText* languageChoice = new wxStaticText(panel, wxID_ANY, staticText1);
	languageChoice->SetPosition(wxPoint(20, 20));

	languageChoice->SetSize(wxSize(50, 40));

	wxFont font(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

	languageChoice->SetFont(font);
}

void SettingsFrame::UI_FinishButton()
{
	wxString ButtonText = _("finish");
	FinishButton = new wxButton(panel, wxID_ANY, ButtonText);
	FinishButton->SetPosition(wxPoint(250, 400));
	FinishButton->SetSize(wxSize(100, 40));

	wxFont buttonFont = FinishButton->GetFont();
	buttonFont.SetPointSize(15);
	FinishButton->SetFont(buttonFont);
}

void SettingsFrame::UI_languageChoice()
{
	wxString ini_filename = Settings_SRV::GetIniFilePath();
	wxFileConfig* config = new wxFileConfig(wxEmptyString, wxEmptyString, ini_filename);
	int value;
	config->Read(SettingKeys::language, &value);

	wxArrayString choices;
	choices.Add(_("Auto (System Default)"));
	choices.Add("English");
	choices.Add(wxT("简体中文"));

	languageChoice = new wxComboBox(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, choices, wxCB_DROPDOWN);
	languageChoice->SetPosition(wxPoint(100, 20));
	languageChoice->SetSelection(0);
	if (value == wxLANGUAGE_UNKNOWN) {
		languageChoice->SetSelection(0);
	}
	else if (value == wxLANGUAGE_ENGLISH_US) {
		languageChoice->SetSelection(1);
	}
	else if (value == wxLANGUAGE_CHINESE_SIMPLIFIED) {
		languageChoice->SetSelection(2);
	}

	delete config;
}

void SettingsFrame::UI_BindEventHandlers()
{
	FinishButton->Bind(wxEVT_BUTTON, &SettingsFrame::UI_OnFinshButtonClick, this);
	Bind(wxEVT_CLOSE_WINDOW, &SettingsFrame::UI_OnClose, this);
}

void SettingsFrame::UI_OnClose(wxCloseEvent& event)
{
	if (_parentButton != nullptr) {
		_parentButton->Enable(true);
	}
	event.Skip();
}

void SettingsFrame::UI_OnLanguageChange()
{
	int selectedIndex = languageChoice->GetSelection();

	Settings_SRV::WriteLanguage(selectedIndex);
}

void SettingsFrame::UI_OnFinshButtonClick(wxCommandEvent& event)
{
	UI_OnLanguageChange();
	Destroy();
}
