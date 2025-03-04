#pragma once
#include <wx/wx.h>

class UHMTUFrame : public wxFrame
{
public:
	UHMTUFrame(wxButton* parentButton);
private:
	wxButton* _parentButton = nullptr;
	void OnClose(wxCloseEvent& event);
};