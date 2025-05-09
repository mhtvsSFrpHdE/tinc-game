#include "App.h"
#include "MainFrame.h"
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/fileconf.h>
#include <wx/config.h>
#include "Settings_SRV.h"

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	// Config file
	Settings_SRV::LoadConfigFile();

	// Language
	wxLanguage language = Settings_SRV::ReadLanguage();
	locale = new wxLocale(language, wxLOCALE_LOAD_DEFAULT);

#ifdef __WXGTK__
	// add locale search paths
	locale->AddCatalogLookupPathPrefix(wxT("/usr"));
	locale->AddCatalogLookupPathPrefix(wxT("/usr/local"));
	wxStandardPaths* paths = (wxStandardPaths*)&wxStandardPaths::Get();
	wxString prefix = paths->GetInstallPrefix();
	locale->AddCatalogLookupPathPrefix(prefix);
#endif

	bool localeAllOk = true;
	localeAllOk = localeAllOk && locale->AddCatalog(wxT("tinc-game-MainFrame"));
	localeAllOk = localeAllOk && locale->AddCatalog(wxT("tinc-game-OptimizeMtuFrame"));
	localeAllOk = localeAllOk && locale->AddCatalog(wxT("tinc-game-SettingsFrame"));

	localeAllOk = localeAllOk && locale->IsOk();
	if (localeAllOk == false)
	{
		std::cerr << "selected language is wrong" << std::endl;
		delete locale;
		locale = new wxLocale(wxLANGUAGE_ENGLISH_US);
		language = wxLANGUAGE_ENGLISH_US;
	}

	MainFrame* mainFrame = new MainFrame();
	mainFrame->Center();
	mainFrame->Show();
	return true;
}
