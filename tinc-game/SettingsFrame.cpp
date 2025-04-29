#include <wx/wx.h>
#include "SettingsFrame.h"
#include <wx/spinctrl.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/fileconf.h>
#include <wx/config.h>
#include "Settings_SRV.h"

SettingsFrame::SettingsFrame(MainFrame* parentFrame) : wxFrame(parentFrame, wxID_ANY, _("Settings"))
{
	Init_CreateControls();
	Init_BindEventHandlers();
}

void SettingsFrame::Init_CreateControls()
{
	{
		wxStaticText* chooseLanguage_StaticText = new wxStaticText(panel, wxID_ANY, _("language"));
		chooseLanguage_StaticText->SetPosition(wxPoint(20, 20));
		chooseLanguage_StaticText->SetSize(wxSize(50, 40));
		wxFont font(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		chooseLanguage_StaticText->SetFont(font);
	}

	{
		chooseLanguage_ComboBox = new wxComboBox(panel, wxID_ANY);
		chooseLanguage_ComboBox->SetPosition(wxPoint(100, 20));
		chooseLanguage_ComboBox->Append(_("Auto (System Default)"));
		chooseLanguage_ComboBox->Append(wxT("English"));
		chooseLanguage_ComboBox->Append(wxT("简体中文"));

		int value;
		Settings_SRV::config->Read(SettingKeys::language, &value);
		if (value == wxLANGUAGE_ENGLISH_US) {
			chooseLanguage_ComboBox->SetSelection(1);
		}
		else if (value == wxLANGUAGE_CHINESE_SIMPLIFIED) {
			chooseLanguage_ComboBox->SetSelection(2);
		}
		else {
			chooseLanguage_ComboBox->SetSelection(0);
		}
	}

	{
		confirmButton = new wxButton(panel, wxID_ANY, _("Confirm"));
		confirmButton->SetPosition(wxPoint(250, 400));
		confirmButton->SetSize(wxSize(100, 40));

		wxFont font = confirmButton->GetFont();
		font.SetPointSize(15);
		confirmButton->SetFont(font);
	}
}

void SettingsFrame::Init_BindEventHandlers()
{
	confirmButton->Bind(wxEVT_BUTTON, &SettingsFrame::OnConfirmButtonClick, this);
}

void SettingsFrame::OnConfirmButtonClick(wxCommandEvent& event)
{
	int selectedIndex = chooseLanguage_ComboBox->GetSelection();
	Settings_SRV::WriteLanguage(selectedIndex);

	Close();
}
