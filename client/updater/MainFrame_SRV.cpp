#include <boost/process.hpp>
#include <boost/process/windows.hpp>
#include "MainFrame.h"
#include "FileSystem_SRV.h"
#include "../resource/Resource_SRV_Program.h"
#include "../resource/Resource_SRV_Bat.h"
#include <fstream>

void MainFrame::API_SRV_PrepareUpdate_EndLogToFile()
{
    CallAfter([this]() {
        wxLog::FlushActive();
        wxLog::SetActiveTarget(nullptr);
        Lock_SRV::Notify(uiMutex, uiCb);
        });
    Lock_SRV::Wait(uiMutex, uiCb);
}

void MainFrame::API_SRV_PrepareUpdate_OnCopyFail(ReturnValue<PrepareUpdateResult>& result, wxFileName& srcfile, wxFileName& destfile)
{
    result.returnBody.messageEnum = PrepareUpdateResult::Enum::Copy;
    result.returnBody.messageString = srcfile.GetFullPath() + " to " + destfile.GetFullPath();
    API_SRV_PrepareUpdate_EndLogToFile();
    CallAfter([this, result]() {
        API_UI_ReportPrepareUpdateResult(result);
        });
}

void MainFrame::API_SRV_PrepareUpdate_OnRenameFail(ReturnValue<PrepareUpdateResult>& result, wxFileName& srcfile, wxFileName& destfile)
{
    result.returnBody.messageEnum = PrepareUpdateResult::Enum::Rename;
    result.returnBody.messageString = srcfile.GetFullPath() + " to " + destfile.GetFullPath();
    API_SRV_PrepareUpdate_EndLogToFile();
    CallAfter([this, result]() {
        API_UI_ReportPrepareUpdateResult(result);
        });
}

void MainFrame::API_SRV_PrepareUpdate_OnRemoveFail(ReturnValue<PrepareUpdateResult>& result, wxFileName& file)
{
    result.returnBody.messageEnum = PrepareUpdateResult::Enum::Remove;
    result.returnBody.messageString = file.GetFullPath();
    API_SRV_PrepareUpdate_EndLogToFile();
    CallAfter([this, result]() {
        API_UI_ReportPrepareUpdateResult(result);
        });
}

bool MainFrame::API_SRV_PrepareUpdate()
{
    ReturnValue<PrepareUpdateResult> result;

    namespace rs = Resource_SRV;
    namespace fs = FileSystem_SRV;

    std::ofstream outFile(rs::Program::updaterLog, std::ios::app);
    auto logTarget = std::unique_ptr<wxLogStream>(new wxLogStream(&outFile));
    if (outFile.is_open())
    {
        wxLog::SetActiveTarget(logTarget.get());
    }
    else
    {
        result.returnBody.messageEnum = PrepareUpdateResult::Enum::NoLogFile;
        CallAfter([this, result]() {
            API_UI_ReportPrepareUpdateResult(result);
            });
        return false;
    }

    wxLogMessage("==== Start Prepare update");

    {
        auto binDir = rs::Program::GetBinDir();
        bool removeResult = fs::tRemoveDir(binDir);
        if (removeResult == false) {
            API_SRV_PrepareUpdate_OnRemoveFail(result, binDir);
            return false;
        }
    }

    auto programDir = rs::Program::GetProgramDir();
    {
        auto batGetTapHwid = programDir;
        batGetTapHwid.SetName(rs::Bat::getTapHwid);
        bool removeResult = fs::tRemoveFile(batGetTapHwid);
        if (removeResult == false) {
            API_SRV_PrepareUpdate_OnRemoveFail(result, batGetTapHwid);
            return false;
        }
    }
    {
        auto batInstallTap = programDir;
        batInstallTap.SetName(rs::Bat::installTap);
        bool removeResult = fs::tRemoveFile(batInstallTap);
        if (removeResult == false) {
            API_SRV_PrepareUpdate_OnRemoveFail(result, batInstallTap);
            return false;
        }
    }
    {
        auto batNetsh437 = programDir;
        batNetsh437.SetName(rs::Bat::netsh437);
        bool removeResult = fs::tRemoveFile(batNetsh437);
        if (removeResult == false) {
            API_SRV_PrepareUpdate_OnRemoveFail(result, batNetsh437);
            return false;
        }
    }
    {
        auto batNetsh437Start = programDir;
        batNetsh437Start.SetName(rs::Bat::netsh437Start);
        bool removeResult = fs::tRemoveFile(batNetsh437Start);
        if (removeResult == false) {
            API_SRV_PrepareUpdate_OnRemoveFail(result, batNetsh437Start);
            return false;
        }
    }
    {
        auto batNetsh437Result = programDir;
        batNetsh437Result.SetName(rs::Bat::netsh437Result);
        std::ignore = fs::tRemoveFile(batNetsh437Result);
    }
    {
        auto batPing437 = programDir;
        batPing437.SetName(rs::Bat::ping437);
        bool removeResult = fs::tRemoveFile(batPing437);
        if (removeResult == false) {
            API_SRV_PrepareUpdate_OnRemoveFail(result, batPing437);
            return false;
        }
    }
    {
        auto batPing437Start = programDir;
        batPing437Start.SetName(rs::Bat::ping437Start);
        bool removeResult = fs::tRemoveFile(batPing437Start);
        if (removeResult == false) {
            API_SRV_PrepareUpdate_OnRemoveFail(result, batPing437Start);
            return false;
        }
    }
    {
        auto batPing437Result = programDir;
        batPing437Result.SetName(rs::Bat::ping437Result);
        std::ignore = fs::tRemoveFile(batPing437Result);
    }
    {
        auto tincGameExe = programDir;
        tincGameExe.SetName(rs::Program::programExe);
        bool removeResult = fs::tRemoveFile(tincGameExe);
        if (removeResult == false) {
            API_SRV_PrepareUpdate_OnRemoveFail(result, tincGameExe);
            return false;
        }
    }
    {
        auto batTincShell = programDir;
        batTincShell.SetName(rs::Bat::tincShell);
        bool removeResult = fs::tRemoveFile(batTincShell);
        if (removeResult == false) {
            API_SRV_PrepareUpdate_OnRemoveFail(result, batTincShell);
            return false;
        }
    }
    {
        auto batUninstallTap = programDir;
        batUninstallTap.SetName(rs::Bat::uninstallTap);
        bool removeResult = fs::tRemoveFile(batUninstallTap);
        if (removeResult == false) {
            API_SRV_PrepareUpdate_OnRemoveFail(result, batUninstallTap);
            return false;
        }
    }

    wxLogMessage("End Prepare update");

    API_SRV_PrepareUpdate_EndLogToFile();
    return true;
}

void MainFrame::API_SRV_ProcessAdditionalArgument()
{
    bool allSuccess = true;
    if (allSuccess && command_prepareUpdate) {
        allSuccess = API_SRV_PrepareUpdate();
    }
    if (allSuccess && command_uninstall) {
        namespace bp = boost::process;
        namespace rs = Resource_SRV;

        wxMessageDialog(this, L"Your user data isn't being removed\nclick OK to open user data directory").ShowModal();

        auto programDir = rs::Program::GetProgramDir();
        bp::system(bp::shell(), bp::args({ rs::Bat::cmdRumCommand, L"explorer.exe", programDir.GetFullPath().ToStdWstring() }), bp::windows::hide);
    }

    CallAfter([this]() {
        Close();
        });
}
