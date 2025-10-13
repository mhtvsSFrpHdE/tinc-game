#include "MainFrame.h"
#include "App.h"
#include <wx/stdpaths.h>
#include <wx/fileconf.h>
#include <wx/config.h>
#include "Settings_SRV.h"

wxIMPLEMENT_APP(App);

bool App::OnInit() {
    namespace ss = Settings_SRV;

    // https://docs.wxwidgets.org/trunk/classwx_app_console.html
    if (!wxApp::OnInit()) {
        // The most likely reason for the error here is that incorrect
        // command line arguments have been specified, so just exit:
        // error message has already been given.
        return false;
    }

    // Config file
    ss::LoadConfigFile();

    // Language
    wxLanguage language = ss::ReadLanguage();
    locale = std::shared_ptr<wxLocale>(new wxLocale(language, wxLOCALE_LOAD_DEFAULT));

    bool localeAllOk = true;
    localeAllOk = localeAllOk && locale->AddCatalog(wxT("tinc-game-MainFrame"));
    localeAllOk = localeAllOk && locale->AddCatalog(wxT("tinc-game-OptimizeMtuFrame"));
    localeAllOk = localeAllOk && locale->AddCatalog(wxT("tinc-game-SettingsFrame"));

    localeAllOk = localeAllOk && locale->IsOk();
    if (localeAllOk == false)
    {
        locale.reset();
        locale = std::shared_ptr<wxLocale>(new wxLocale(wxLANGUAGE_ENGLISH_US));
        language = wxLANGUAGE_ENGLISH_US;
    }

    MainFrame* mainFrame = new MainFrame();
    mainFrame->Center();
    mainFrame->Show();
    return true;
}
