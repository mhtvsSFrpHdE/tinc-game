#pragma once
#include <wx/wx.h>

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
private:
	static const std::wstring frameTitle;
	wxButton* _parentButton = nullptr;
	void UI_OnClose(wxCloseEvent& event);

	void UI_CreateControls();

	wxPanel* panel = new wxPanel(this);
};