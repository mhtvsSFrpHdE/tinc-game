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
	MainFrame* _parentFrame = nullptr;

	wxPanel* panel = new wxPanel(this);

	wxComboBox* chooseTargetAddress_ComboBox;
	wxButton* startButton;
	wxButton* closeButton;
	wxStaticText* mtuValue_IPv4 = nullptr;
	wxStaticText* mtuValue_IPv6 = nullptr;
	const wxString mtuValue_DefaultText = _("Waiting for value...");
	wxTextCtrl* liveLog;

	void Init_CreateControls();
	void Init_BindEventHandlers();

	void OnClose(wxCloseEvent& event);
	void OnStartButtonClick(wxCommandEvent& event);
	void OnCloseButtonClick(wxCommandEvent& event);

	int reportMtuCount_IPv4 = 0;
	int reportMtuCount_IPv6 = 0;
};

class OptimizeMtuFrameTest {
public:
	static void TestCheckAddressFormat(wxWindow* parent);
};
