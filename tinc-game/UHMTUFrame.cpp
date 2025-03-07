#include "UHMTUFrame.h"
#include <wx/spinctrl.h>

UHMTUFrame::UHMTUFrame(wxButton* parentButton) : wxFrame(nullptr, wxID_ANY, frameTitle) {
	_parentButton = parentButton;

	Bind(wxEVT_CLOSE_WINDOW, &UHMTUFrame::UI_OnClose, this);
	UI_CreateControls();
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
	wxStaticText* staticText = new wxStaticText(panel, wxID_ANY, "Hello, this is a static text!", wxPoint(20, 20));
}

const std::wstring UHMTUFrame::UHMTUFrame::frameTitle = L"优化MTU";