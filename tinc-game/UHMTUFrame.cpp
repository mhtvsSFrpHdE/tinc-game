#include "UHMTUFrame.h"
#include <wx/spinctrl.h>

UHMTUFrame::UHMTUFrame(wxButton* parentButton) : wxFrame(nullptr, wxID_ANY, frameTitle) {
	_parentButton = parentButton;

	Bind(wxEVT_CLOSE_WINDOW, &UHMTUFrame::UI_OnClose, this);
	UI_CreateControls();
	SRV_TestButton->Bind(wxEVT_BUTTON, &UHMTUFrame::SRV_TestCheckAddressFormat, this);
}
void UHMTUFrame::UI_OnClose(wxCloseEvent& event)
{
	if (_parentButton != nullptr) {
		_parentButton->Enable(true);
	}
	event.Skip();
}

void UHMTUFrame::UI_CreateControls()
{
	SRV_LiveLog = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxPoint(0, 100), wxSize(320,240), wxTE_READONLY | wxTE_MULTILINE);
	SRV_LiveMtu = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxPoint(340, 100), wxSize(320, 240), wxTE_READONLY | wxTE_MULTILINE);
	SRV_TestButton = new wxButton(panel, wxID_ANY, "Test", wxPoint(0, 0));
}

const std::wstring UHMTUFrame::UHMTUFrame::frameTitle = L"优化MTU";