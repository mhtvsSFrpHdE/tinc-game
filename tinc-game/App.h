#pragma once
#include <wx/wx.h>

class App : public wxApp
{
public:
	bool OnInit();
private:
	wxLocale* locale;
	long language;
};
