#pragma once
#include <wx/wx.h>
#include <thread>
#include "ReturnValue.h"
#include "MainFrame.h"

struct CheckAddressFormatResult {
	enum Enum {
		CheckAddressFormat_Other,
		CheckAddressFormat_NotIPv4,
		CheckAddressFormat_NotDomain
	};
};

class OptimizeMtuFrame : public wxFrame
{
public:
	OptimizeMtuFrame(MainFrame* parentFrame, wxString frameTitle);

	// UI to SRV
	static ReturnValue<CheckAddressFormatResult::Enum> API_SRV_CheckAddressFormat(std::wstring ipAddress);
	void API_SRV_StartMeasureMTU(std::wstring ipAddress);

	// SRV to UI
	void API_UI_ReportStatus(std::wstring status);
	void API_UI_ReportMTU_IPv4(int mtu);
	void API_UI_ReportMTU_IPv6(int mtu);
	void API_UI_EndMeasureMTU(bool success, std::wstring reason);

private:
	//Measure
	int attemptNumber_IPv4 = 0;
	int attemptNumber_IPv6 = 0;
	bool pass = false;
	wxString DefaultState = _("Waiting for value...");

	wxPanel* panel = new wxPanel(this);
	wxButton* _parentButton = nullptr;
	MainFrame* _parentFrame = nullptr;

	wxComboBox* chooseTargetAddress_ComboBox;
	wxButton* startButton;
	wxButton* closeButton;
	wxStaticText* mtuValue_IPv4 = nullptr;
	wxStaticText* mtuValue_IPv6 = nullptr;
	wxTextCtrl* liveLog;

	bool Judgment = false;

	wxStaticText* staticText2;
	int isOptionChanged = 2;

	//UI_Event
	void UI_OnClose(wxCloseEvent& event);
	void UI_OnComboBoxSelect(wxCommandEvent& event);
	void UI_OnStartButtonClick(wxCommandEvent& event);
	void UI_OnCloseButtonClick(wxCommandEvent& event);

	//UIControls
	void UI_CreateControls();
	void UI_BindEventHandlers();
};

class OptimizeMtuFrameTest {
public:
	static void TestCheckAddressFormat(wxWindow* parent);
};