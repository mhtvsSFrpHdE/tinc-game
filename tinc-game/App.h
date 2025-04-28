#pragma once
#include <wx/wx.h>

class App : public wxApp
{
public:
	bool OnInit();
	wxLanguage GetLanguage() const;
	void SetLanguage(wxLanguage newLanguage);
private:
	wxLocale* locale;
};
