#pragma once
#include <wx/wx.h>
#include <thread>
#include "ReturnValue.h"
#include "MainFrame.h"

struct OptionWithID {
	int id;
	wxString label;
};

struct CheckAddressFormatResult {
	enum Enum {
		CheckAddressFormat_Other,
		CheckAddressFormat_NotIPv4,
		CheckAddressFormat_NotDomain
	};
};

class UHMTUFrame : public wxFrame
{
public:
	UHMTUFrame(MainFrame* parentFrame, wxString frameTitle);

	// UI to SRV
	static ReturnValue<CheckAddressFormatResult::Enum> API_SRV_CheckAddressFormat(std::wstring ipAddress);
	void API_SRV_StartMeasureMTU(std::wstring ipAddress);

	// SRV to UI
	void API_UI_ReportStatus(std::wstring status);
	void API_UI_ReportMTU_IPv4(int mtu);
	void API_UI_ReportMTU_IPv6(int mtu);
	void API_UI_EndMeasureMTU(bool success, std::wstring reason);
	wxPanel* panel = new wxPanel(this);

	//Measure
	int attemptNumber_IPv4 = 0;
	int attemptNumber_IPv6 = 0;
	bool pass = false;

private:
	wxButton* _parentButton = nullptr;
	MainFrame* _parentFrame = nullptr;

	wxComboBox* m_comboBox;
	wxButton* beginButton;
	wxArrayString choices;
	wxStaticText* nowState_IPv4 = nullptr;
	wxStaticText* nowState_IPv6 = nullptr;
	wxTextCtrl* textCtrl;
	wxString DefaultState;

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