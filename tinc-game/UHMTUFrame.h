#pragma once
#include <wx/wx.h>
#include <thread>

struct OptionWithID {
	int id;
	wxString label;
};

class UHMTUFrame : public wxFrame
{
public:
	UHMTUFrame(wxButton* parentButton);

	// UI to SRV
	static bool API_CheckAddressFormat(std::wstring ipAddress);
	void API_StartMeasureMTU(std::wstring ipAddress);

	// SRV to UI
	void API_ReportStatus(std::wstring status);
	void API_ReportMTU_IPv4(int mtu);
	void API_ReportMTU_IPv6(int mtu);
	void API_EndMeasureMTU(bool success, std::wstring reason);
	wxPanel* panel = new wxPanel(this);

private:
	static const std::wstring frameTitle;
	wxButton* _parentButton = nullptr;

	wxComboBox* m_comboBox;
	wxButton* beginButton;
	wxArrayString choices;

	bool Judgment = false;

	wxStaticText* staticText2;
	int isOptionChanged = 2;

	//UI_Event
	void UI_OnClose(wxCloseEvent& event);
	void UI_OnComboBoxSelect(wxCommandEvent& event);
	void UI_OnStartButtonClick(wxCommandEvent& event);

	//UIControls
	void UI_CreateControls();
	void UI_staticText();
	void UI_HintButton();
	void UI_UserTextCtrl();
	void UI_BeginButton();
	void UI_SystemPointText();
	void UI_StaticTextIP();
	void UI_IPState();
	void UI_CloseButton();

	void UI_BindEventHandlers();

};

class UHMTUFrameTest {
public:
	static void TestCheckAddressFormat(wxWindow* parent);
};