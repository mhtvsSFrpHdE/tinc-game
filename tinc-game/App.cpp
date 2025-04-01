#include "App.h"
#include "MainFrame.h"
// #include <wx/wx.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	// Language
	language = wxLANGUAGE_CHINESE_SIMPLIFIED;
	//language = wxLANGUAGE_ENGLISH;

	locale = new wxLocale(language, wxLOCALE_LOAD_DEFAULT);

#ifdef __WXGTK__
	// add locale search paths
	locale->AddCatalogLookupPathPrefix(wxT("/usr"));
	locale->AddCatalogLookupPathPrefix(wxT("/usr/local"));
	wxStandardPaths* paths = (wxStandardPaths*)&wxStandardPaths::Get();
	wxString prefix = paths->GetInstallPrefix();
	locale->AddCatalogLookupPathPrefix(prefix);
#endif

	locale->AddCatalog(wxT("tinc-game"));

	if (locale->IsOk() == false)
	{
		std::cerr << "selected language is wrong" << std::endl;
		delete locale;
		locale = new wxLocale(wxLANGUAGE_ENGLISH);
		language = wxLANGUAGE_ENGLISH;
	}

	MainFrame* mainFrame = new MainFrame(_("Tinc Game Mode"));
	mainFrame->SetClientSize(800, 600);
	mainFrame->Center();
	mainFrame->Show();
	return true;
}



