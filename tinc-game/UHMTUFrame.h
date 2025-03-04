#pragma once
#include <wx/wx.h>

class UHMTUFrame : public wxFrame
{
public:
	UHMTUFrame(wxButton* parentButton);
private:
	static const std::wstring frameTitle;
	wxButton* _parentButton = nullptr;
	void OnClose(wxCloseEvent& event);
};