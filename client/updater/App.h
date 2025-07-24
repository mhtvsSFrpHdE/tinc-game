#pragma once
#include <wx/wx.h>

class App : public wxApp
{
public:
    bool OnInit() override;
    void OnInitCmdLine(wxCmdLineParser& parser) override;
    bool OnCmdLineParsed(wxCmdLineParser& parser) override;

    bool command_prepareUpdate;
    bool command_uninstall;
};
