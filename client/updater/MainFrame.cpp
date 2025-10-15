#include <boost/process.hpp>
#include <boost/process/windows.hpp>
#include "MainFrame.h"
#include <thread>
#include "../resource/Resource_SRV_Program.h"
#include "../resource/Resource_SRV_Bat.h"
#include "../resource/String_SRV.h"
#include <sstream>

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
    std::thread t1(&MainFrame::API_SRV_Update, this);
    t1.detach();
}

void MainFrame::API_UI_ReportUpdateResult(ReturnValue<UpdateResult> result)
{
    if (result.success) {
        return;
    }

    namespace bp = boost::process;
    namespace rs = Resource_SRV;
    namespace ss = String_SRV;

    std::wstringstream errorMessageStream;
    if (result.returnBody.messageEnum == UpdateResult::Enum::NoLogFile) {
        errorMessageStream << "Failed to create log file";
    }
    bool fileOperationFailed = result.returnBody.messageEnum == UpdateResult::Enum::Copy
        || result.returnBody.messageEnum == UpdateResult::Enum::Rename
        || result.returnBody.messageEnum == UpdateResult::Enum::Remove;
    if (fileOperationFailed) {
        if (result.returnBody.messageEnum == UpdateResult::Enum::Copy) {
            errorMessageStream << "Failed to copy"
                << ss::space << result.returnBody.messageString;
        }
        if (result.returnBody.messageEnum == UpdateResult::Enum::Rename) {
            errorMessageStream << "Failed to rename"
                << ss::space << result.returnBody.messageString;
        }
        if (result.returnBody.messageEnum == UpdateResult::Enum::Remove) {
            errorMessageStream << "Failed to remove"
                << ss::space << result.returnBody.messageString;
        }
        errorMessageStream << ss::newLine
            << ss::newLine
            << "Your installation seems corrupted" << ss::newLine
            << "Move all files in install folder to somewhere else" << ss::newLine
            << "then try to install from scratch" << ss::newLine
            << ss::newLine
            << "Do not delete them before your restored critical data to new installation";
    }

    auto errorMessage = errorMessageStream.str();
    wxMessageDialog(this, errorMessage).ShowModal();

    auto programDir = rs::Program::GetProgramDir();
    bp::system(bp::shell(), bp::args({ rs::Bat::cmdRumCommand, L"explorer.exe", programDir.GetFullPath().ToStdWstring() }), bp::windows::hide);

    returnCode = 1;
    Close();
}

void MainFrame::API_UI_ReportPrepareUpdateResult(ReturnValue<PrepareUpdateResult> result)
{
    if (result.success) {
        return;
    }

    namespace bp = boost::process;
    namespace rs = Resource_SRV;
    namespace ss = String_SRV;

    std::wstringstream errorMessageStream;
    if (result.returnBody.messageEnum == PrepareUpdateResult::Enum::NoLogFile) {
        errorMessageStream << "Failed to create log file";
    }
    bool fileOperationFailed = result.returnBody.messageEnum == PrepareUpdateResult::Enum::Copy
        || result.returnBody.messageEnum == PrepareUpdateResult::Enum::Rename
        || result.returnBody.messageEnum == PrepareUpdateResult::Enum::Remove;
    if (fileOperationFailed) {
        if (result.returnBody.messageEnum == PrepareUpdateResult::Enum::Copy) {
            errorMessageStream << "Failed to copy"
                << ss::space << result.returnBody.messageString;
        }
        if (result.returnBody.messageEnum == PrepareUpdateResult::Enum::Rename) {
            errorMessageStream << "Failed to rename"
                << ss::space << result.returnBody.messageString;
        }
        if (result.returnBody.messageEnum == PrepareUpdateResult::Enum::Remove) {
            errorMessageStream << "Failed to remove"
                << ss::space << result.returnBody.messageString;
        }
        errorMessageStream << ss::newLine
            << ss::newLine
            << "Your installation seems corrupted" << ss::newLine
            << "Move all files in install folder to somewhere else" << ss::newLine
            << "then try to install from scratch" << ss::newLine
            << ss::newLine
            << "Do not delete them before your restored critical data to new installation";
    }

    auto errorMessage = errorMessageStream.str();
    wxMessageDialog(this, errorMessage).ShowModal();

    auto programDir = rs::Program::GetProgramDir();
    bp::system(bp::shell(), bp::args({ rs::Bat::cmdRumCommand, L"explorer.exe", programDir.GetFullPath().ToStdWstring() }), bp::windows::hide);

    Close();
}

bool MainFrame::command_prepareUpdate;
bool MainFrame::command_uninstall;
int MainFrame::returnCode = 0;
