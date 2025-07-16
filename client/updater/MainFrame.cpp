#include "MainFrame.h"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, _("Tinc Game updater")) {
    SetSizeHints(600, 400);
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* text = new wxStaticText(this, wxID_ANY, "Well Done!\nEverything seems to be working",
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    text->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    sizer->Add(text, 1, wxALIGN_CENTER);
    this->SetSizer(sizer);
}
