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
	Init_BindEventHandlers();
	Init_Layout();
}

void SettingsFrame::Init_CreateControls()
{
	chooseLanguage_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Language"));

	{
		chooseLanguage_ComboBox = new wxComboBox(rootPanel, wxID_ANY);
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

	confirmButton = new wxButton(rootPanel, wxID_ANY, _("Confirm"));
}

void SettingsFrame::Init_BindEventHandlers()
{
	confirmButton->Bind(wxEVT_BUTTON, &SettingsFrame::OnConfirmButtonClick, this);
}

void SettingsFrame::Init_Layout()
{
	namespace ls = Layout_SRV;

	this->SetSizeHints(320, 160);

	wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
	rootPanel->SetSizer(rootSizer);
	ls::AddSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

	rootSizer->Add(chooseLanguage_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
	ls::AddSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

	rootSizer->Add(chooseLanguage_ComboBox, 0, wxLEFT, ls::SpaceToFrameBorder);
	ls::AddSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

	rootSizer->Add(0, 0, ls::TakeAllSpace, wxEXPAND);
	wxBoxSizer* navigateSizer = new wxBoxSizer(wxHORIZONTAL);
	rootSizer->Add(navigateSizer);
	navigateSizer->AddStretchSpacer(ls::TakeAllSpace);
	navigateSizer->Add(confirmButton, 1, wxRIGHT, ls::SpaceToFrameBorder);

	ls::AddSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);
}

void SettingsFrame::OnConfirmButtonClick(wxCommandEvent& event)
{
	int selectedIndex = chooseLanguage_ComboBox->GetSelection();
	Settings_SRV::WriteLanguage(selectedIndex);

	Close();
}
