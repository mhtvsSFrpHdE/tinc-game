#include "MainFrame.h"
#include <wx/spinctrl.h>
#include "OptimizeMtuFrame.h"
#include "SettingsFrame.h"
#include <wx/dialog.h>
#include "Layout_SRV.h"
#include "ManageTapFrame.h"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, _("Tinc Game Mode")) {
    Init_CreateControls();
    Init_BindEventHandlers();
    Init_Layout();
}

void MainFrame::Init_CreateControls()
{
    rootPanel = new wxPanel(this);
    optimizeMtu_Button = new wxButton(rootPanel, wxID_ANY, _("Optimize MTU"));
    manageTapDevice_Button = new wxButton(rootPanel, wxID_ANY, _("Manage virtual network adapter"));
    settings_Button = new wxButton(rootPanel, wxID_ANY, _("Settings"));
}

void MainFrame::Init_BindEventHandlers()
{
    optimizeMtu_Button->Bind(wxEVT_BUTTON, &MainFrame::OnOptimizeMtuButton, this);
    manageTapDevice_Button->Bind(wxEVT_BUTTON, &MainFrame::OnManageTapButton, this);
    settings_Button->Bind(wxEVT_BUTTON, &MainFrame::OnSettingsButton, this);
}

void MainFrame::Init_Layout()
{
    namespace ls = Layout_SRV;

    wxSize minSize(280, 170);
    this->SetSizeHints(minSize);

    wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
    rootPanel->SetSizer(rootSizer);
    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    wxBoxSizer* optimizeMtuSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(optimizeMtuSizer, 1, wxEXPAND);
    optimizeMtuSizer->Add(0, 0, 0, wxLEFT, ls::SpaceToFrameBorder);
    optimizeMtuSizer->Add(optimizeMtu_Button, 1);
    optimizeMtuSizer->Add(0, 0, 0, wxRIGHT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    wxBoxSizer* manageTapDeviceSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(manageTapDeviceSizer, 1, wxEXPAND);
    manageTapDeviceSizer->Add(0, 0, 0, wxLEFT, ls::SpaceToFrameBorder);
    manageTapDeviceSizer->Add(manageTapDevice_Button, 1);
    manageTapDeviceSizer->Add(0, 0, 0, wxRIGHT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    wxBoxSizer* settingsSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(settingsSizer, 1, wxEXPAND);
    settingsSizer->Add(0, 0, 0, wxLEFT, ls::SpaceToFrameBorder);
    settingsSizer->Add(settings_Button, 1);
    settingsSizer->Add(0, 0, 0, wxRIGHT, ls::SpaceToFrameBorder);

    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    this->SetSize(minSize);
}

void MainFrame::OnOptimizeMtuButton(wxCommandEvent& evt)
{
    auto button = (wxButton*)evt.GetEventObject();
    button->Enable(false);

    OnOptimizeMtuButton_OpenOptimizeMtuFrame();
}

void MainFrame::OnOptimizeMtuButton_OpenOptimizeMtuFrame()
{
    OptimizeMtuFrame* optimizeMtuframe = new OptimizeMtuFrame(this);
    optimizeMtuframe->Center();
    optimizeMtuframe->Show();
}

void MainFrame::OnManageTapButton(wxCommandEvent& evt)
{
    ManageTapFrame* manageTapDeviceFrame = new ManageTapFrame(this);
    manageTapDeviceFrame->Center();
    manageTapDeviceFrame->Show();
}

void MainFrame::OnSettingsButton(wxCommandEvent& evt)
{
    if (openedFrameCount == 0)
    {
        OnSettingsButton_OpenSettingsFrame();
    }
    else {
        OnSettingsButton_OtherWindowExists();
    }
}

void MainFrame::OnSettingsButton_OpenSettingsFrame()
{
    SettingsFrame* settingsframe = new SettingsFrame(this);
    settingsframe->Center();
    settingsframe->Show();
}

void MainFrame::OnSettingsButton_OtherWindowExists()
{
    wxString buttonHint = _("Close all windows before enter setting interface.");
    wxString title = _("hint");
    wxMessageDialog* dial = new wxMessageDialog(NULL,
        buttonHint, title, wxOK);
    dial->ShowModal();
}
