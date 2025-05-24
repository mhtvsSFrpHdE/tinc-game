#include "MainFrame.h"
#include <wx/spinctrl.h>
#include "OptimizeMtuFrame.h"
#include "SettingsFrame.h"
#include <wx/dialog.h>
#include "Layout_SRV.h"
#include "ManageTapFrame.h"
#include <wx/filename.h>
#include <sstream>
#include "String_SRV.h"
#include "HelpFrame.h"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, _("Tinc Game Mode")) {
    Init_CreateControls();
    Init_Layout();
}

void MainFrame::Init_CreateControls()
{
    rootPanel = new wxPanel(this);
    currentNetwork_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Current virtual network"));
    currentNetwork_ComboBox = new wxComboBox(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY);
    {
        // TODO: fill data from settings
        // TODO: get exist network, may contains network not created by tinc-game
    }
    optimizeMtuButton = new wxButton(rootPanel, wxID_ANY, _("Optimize MTU"));
    optimizeMtuButton->Bind(wxEVT_BUTTON, &MainFrame::OnOptimizeMtuButton, this);
    manageTapDeviceButton = new wxButton(rootPanel, wxID_ANY, _("Manage virtual network adapter"));
    manageTapDeviceButton->Bind(wxEVT_BUTTON, &MainFrame::OnManageTapButton, this);
    integrityCheckButton = new wxButton(rootPanel, wxID_ANY, _("Troubleshoot"));
    integrityCheckButton->Bind(wxEVT_BUTTON, &MainFrame::OnIntegrityCheckButton, this);
    settingsButton = new wxButton(rootPanel, wxID_ANY, _("Settings"));
    settingsButton->Bind(wxEVT_BUTTON, &MainFrame::OnSettingsButton, this);
}

void MainFrame::Init_Layout()
{
    namespace ls = Layout_SRV;

    wxSize minSize(640, 480);
    this->SetSizeHints(minSize);

    wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
    rootPanel->SetSizer(rootSizer);
    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    rootSizer->Add(currentNetwork_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
    rootSizer->Add(currentNetwork_ComboBox, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    wxBoxSizer* optimizeMtuSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(optimizeMtuSizer, 1, wxEXPAND);
    optimizeMtuSizer->Add(0, 0, 0, wxLEFT, ls::SpaceToFrameBorder);
    optimizeMtuSizer->Add(optimizeMtuButton, 1);
    optimizeMtuSizer->Add(0, 0, 0, wxRIGHT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    wxBoxSizer* manageTapDeviceSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(manageTapDeviceSizer, 1, wxEXPAND);
    manageTapDeviceSizer->Add(0, 0, 0, wxLEFT, ls::SpaceToFrameBorder);
    manageTapDeviceSizer->Add(manageTapDeviceButton, 1);
    manageTapDeviceSizer->Add(0, 0, 0, wxRIGHT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    wxBoxSizer* integrityCheckSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(integrityCheckSizer, 1, wxEXPAND);
    integrityCheckSizer->Add(0, 0, 0, wxLEFT, ls::SpaceToFrameBorder);
    integrityCheckSizer->Add(integrityCheckButton, 1);
    integrityCheckSizer->Add(0, 0, 0, wxRIGHT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);

    wxBoxSizer* settingsSizer = new wxBoxSizer(wxHORIZONTAL);
    rootSizer->Add(settingsSizer, 1, wxEXPAND);
    settingsSizer->Add(0, 0, 0, wxLEFT, ls::SpaceToFrameBorder);
    settingsSizer->Add(settingsButton, 1);
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

int OnIntegrityCheckButton_FailedCount = 0;
void OnIntegrityCheckButton_DirExist(std::wostringstream& helpText, wxFileName& file) {
    namespace ss = String_SRV;

    bool exists = file.DirExists();
    helpText << file.GetFullPath() << ss::ellipses
        << (exists ? _("OK") : _("Fail"))
        << ss::newLine;
    if (exists == false) {
        OnIntegrityCheckButton_FailedCount++;
    }
}

void OnIntegrityCheckButton_FileExist(std::wostringstream& helpText, wxFileName& file) {
    namespace ss = String_SRV;

    bool exists = file.Exists();
    helpText << file.GetFullPath() << ss::ellipses
        << (exists ? _("OK") : _("Fail"))
        << ss::newLine;
    if (exists == false) {
        OnIntegrityCheckButton_FailedCount++;
    }
}

void MainFrame::OnIntegrityCheckButton(wxCommandEvent& evt)
{
    integrityCheckButton->Enable(false);
    OnIntegrityCheckButton_FailedCount = 0;

    namespace ss = String_SRV;

    std::wostringstream helpText;
    wxFileName file;

    helpText << _("Program integrity report")
        << ss::newLine << ss::newLine;

    {
        helpText << _("Virtual network adapter driver files:") << ss::newLine;

        file.AppendDir("bin");
        file.AppendDir("tinc");
        file.AppendDir("tap-win64");

        file.SetName(L"tapinstall.exe");
        OnIntegrityCheckButton_FileExist(helpText, file);
        file.SetName(L"OemWin2k.inf");
        OnIntegrityCheckButton_FileExist(helpText, file);
        file.SetName(L"tap0901.cat");
        OnIntegrityCheckButton_FileExist(helpText, file);
        file.SetName(L"tap0901.sys");
        OnIntegrityCheckButton_FileExist(helpText, file);

        helpText << ss::newLine;
        file.Clear();
    }

    {
        helpText << _("Command line I/O files:") << ss::newLine;

        file.SetName(L"getTapHwid.bat");
        OnIntegrityCheckButton_FileExist(helpText, file);
        file.SetName(L"installTap.bat");
        OnIntegrityCheckButton_FileExist(helpText, file);
        file.SetName(L"uninstallTap.bat");
        OnIntegrityCheckButton_FileExist(helpText, file);
        file.SetName(L"netsh437.bat");
        OnIntegrityCheckButton_FileExist(helpText, file);
        file.SetName(L"netsh437_start.bat");
        OnIntegrityCheckButton_FileExist(helpText, file);
        file.SetName(L"ping437.bat");
        OnIntegrityCheckButton_FileExist(helpText, file);
        file.SetName(L"ping437_start.bat");
        OnIntegrityCheckButton_FileExist(helpText, file);

        helpText << ss::newLine;
        file.Clear();
    }

    {
        helpText << _("Translation files:") << ss::newLine;

        file.AppendDir(L"zh");
        OnIntegrityCheckButton_DirExist(helpText, file);

        helpText << ss::newLine;
        file.Clear();
    }

    if (OnIntegrityCheckButton_FailedCount >= 4) {
        helpText << _("Failed items more than 4, don't double click tinc-game in zip/7z/rar files directly you foo")
            << ss::newLine << ss::newLine;
    }

    std::function<void()> redirectCallback = std::bind(&MainFrame::OnIntegrityCheckFrameCloseCallback, this);
    HelpFrame* optimizeMtuFrame_HelpFrame = new HelpFrame(this, _("Troubleshoot"), redirectCallback);
    optimizeMtuFrame_HelpFrame->SetHelpText(helpText.str());
    optimizeMtuFrame_HelpFrame->Center();
    optimizeMtuFrame_HelpFrame->Show();
}

void MainFrame::OnIntegrityCheckFrameCloseCallback()
{
    integrityCheckButton - Enable(true);
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
