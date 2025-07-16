#include "MainFrame.h"
#include <thread>

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, _("Tinc Game updater")) {
    Enable(false);

    Init_CreateControls();
    Init_Layout();
    Init_PostLayout();
}

void MainFrame::Init_CreateControls()
{
    rootPanel = new wxPanel(this);
    text = new wxStaticText(rootPanel, wxID_ANY, "Well Done!\nEverything seems to be working",
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    text->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
}

void MainFrame::Init_Layout()
{
    SetSizeHints(600, 400);

    wxBoxSizer* rootSizer = new wxBoxSizer(wxHORIZONTAL);
    rootPanel->SetSizer(rootSizer);

    rootSizer->Add(text, 1, wxALIGN_CENTER);
}

void MainFrame::Init_PostLayout()
{
    std::thread t1(&MainFrame::API_SRV_PostLayout, this);
    t1.detach();
}
