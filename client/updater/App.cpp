#include "App.h"
#include "MainFrame.h"
#include "Settings_SRV.h"

wxIMPLEMENT_APP(App);

bool App::OnInit() {
    namespace ss = Settings_SRV;

    // Config file
    ss::LoadConfigFile();

    MainFrame* mainFrame = new MainFrame();
    mainFrame->Center();
    return true;
}
