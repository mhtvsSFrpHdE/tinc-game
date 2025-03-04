#include "UHMTUFrame.h"
#include <wx/spinctrl.h>

UHMTUFrame::UHMTUFrame(wxButton* parentButton) : wxFrame(nullptr, wxID_ANY, "UH MTU") {
	_parentButton = parentButton;

	Bind(wxEVT_CLOSE_WINDOW, &UHMTUFrame::OnClose, this);
}

void UHMTUFrame::OnClose(wxCloseEvent& event)
{
	if (_parentButton != nullptr) {
		_parentButton->Enable(true);
	}
    event.Skip();
}
