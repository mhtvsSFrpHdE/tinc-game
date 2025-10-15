#include "App.h"
#include "MainFrame.h"
#include "Settings_SRV.h"
#include <wx/cmdline.h>

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

    MainFrame::command_prepareUpdate = command_prepareUpdate;
    MainFrame::command_uninstall = command_uninstall;
    MainFrame* mainFrame = new MainFrame();
    mainFrame->Center();
    return true;
}

void App::OnInitCmdLine(wxCmdLineParser& parser)
{
    wxApp::OnInitCmdLine(parser);

    parser.AddSwitch(wxT("p"), wxT("prepare-update"), wxT("Delete all program files except config and user data"));
    parser.AddSwitch(wxT("u"), wxT("uninstall"), wxT("Copy user data to desktop, delete all config files and user data, "));
}

bool App::OnCmdLineParsed(wxCmdLineParser& parser)
{
    if (!wxApp::OnCmdLineParsed(parser)) {
        return false;
    }

    command_prepareUpdate = parser.Found(wxT("p"));
    command_uninstall = parser.Found(wxT("u"));

    return true;
}

int App::OnRun()
{
    std::ignore = wxApp::OnRun();
    int returnCode = MainFrame::returnCode;
    return returnCode;
}
