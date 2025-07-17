#include "MainFrame.h"
#include "App.h"
#include <wx/stdpaths.h>
#include <wx/fileconf.h>
#include <wx/config.h>
#include "Settings_SRV.h"
#include "..\resource\Resource_SRV.h"

wxIMPLEMENT_APP(App);

bool App::OnInit() {
    namespace ss = Settings_SRV;

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
