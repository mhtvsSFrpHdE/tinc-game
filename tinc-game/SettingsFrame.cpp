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
	_parentFrame->publicInt++;

	UI_CreateControls();
	UI_BindEventHandlers();

	Bind(wxEVT_CLOSE_WINDOW, &SettingsFrame::UI_OnClose, this);
}




void SettingsFrame::UI_CreateControls()
{
	UI_staticText1();
	UI_languageChoice();
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

void SettingsFrame::UI_languageChoice()
{
	wxString ini_filename = Settings_SRV::GetIniFilePath();
	wxFileConfig* config = new wxFileConfig(wxEmptyString, wxEmptyString, ini_filename);
	int value;
	config->Read(SettingKeys::language, &value);

	wxArrayString choices;
	choices.Add("English");
	wxString chinese = wxT("简体中文");
	//std::wstring chinese = L"简体中文";
	choices.Add(chinese);

	languageChoice = new wxComboBox(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, choices, wxCB_DROPDOWN);
	languageChoice->SetPosition(wxPoint(100, 20));
	languageChoice->SetSelection(0);
	if (value != 0) {
		languageChoice->SetSelection(value);
	}

	delete config;
}

void SettingsFrame::UI_BindEventHandlers()
{
	languageChoice->Bind(wxEVT_COMBOBOX, &SettingsFrame::UI_OnLanguageChenge, this);
}



void SettingsFrame::UI_OnClose(wxCloseEvent& event)
{
	if (_parentButton != nullptr) {
		_parentButton->Enable(true);
	}
	_parentFrame->publicInt--;
	event.Skip();
}

void SettingsFrame::UI_OnLanguageChenge(wxCommandEvent& event)
{
	int selectedIndex = languageChoice->GetSelection();

	Settings_SRV::ChangeLanguage(selectedIndex);
}
