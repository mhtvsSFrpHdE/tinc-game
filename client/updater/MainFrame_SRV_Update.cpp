#include "MainFrame.h"
#include "Settings_SRV.h"
#include "../resource/Resource_SRV.h"
#include <fstream>
#include "File_SRV.h"

long GetInstalledVersion() {
    namespace ss = Settings_SRV;
    namespace sk = SettingKeys_Updater;
    namespace sdn = SettingDefaultValue_Updater;

    return ss::updaterConfig->ReadLong(sk::metadata_installedVersion, sdn::installedVersion);
}

void MainFrame::API_SRV_Update_EndLogToFile()
{
    CallAfter([this]() {
        wxLog::FlushActive();
        wxLog::SetActiveTarget(nullptr);
        Lock_SRV::Notify(uiMutex, uiCb);
        });
    Lock_SRV::Wait(uiMutex, uiCb);
}

void MainFrame::API_SRV_Update_OnCopyFail(ReturnValue<UpdateResult>& result, wxFileName& srcfile, wxFileName& destfile)
{
    result.returnBody.messageEnum = UpdateResult::Enum::Copy;
    result.returnBody.messageString = srcfile.GetFullPath() + " to " + destfile.GetFullPath();
    API_SRV_Update_EndLogToFile();
    CallAfter([this, result]() {
        API_UI_ReportUpdateResult(result);
        });
}

void MainFrame::API_SRV_Update_OnRenameFail(ReturnValue<UpdateResult>& result, wxFileName& srcfile, wxFileName& destfile)
{
    result.returnBody.messageEnum = UpdateResult::Enum::Rename;
    result.returnBody.messageString = srcfile.GetFullPath() + " to " + destfile.GetFullPath();
    API_SRV_Update_EndLogToFile();
    CallAfter([this, result]() {
        API_UI_ReportUpdateResult(result);
        });
}

void MainFrame::API_SRV_Update_OnRemoveFail(ReturnValue<UpdateResult>& result, wxFileName& file)
{
    result.returnBody.messageEnum = UpdateResult::Enum::Remove;
    result.returnBody.messageString = file.GetFullPath();
    API_SRV_Update_EndLogToFile();
    CallAfter([this, result]() {
        API_UI_ReportUpdateResult(result);
        });
}

std::wstring wxFileNameToString(wxFileName& file) {
    return file.GetName().ToStdWstring();
}

void MainFrame::API_SRV_Update()
{
    ReturnValue<UpdateResult> result;

    namespace ss = Settings_SRV;
    namespace sk = SettingKeys_Updater;
    namespace sdn = SettingDefaultValue_Updater;
    namespace rs = Resource_SRV;
    namespace fs = File_SRV;

    std::ofstream outFile(rs::Program::updaterLog);
    auto logTarget = std::unique_ptr<wxLogStream>(new wxLogStream(&outFile));
    if (outFile.is_open())
    {
        wxLog::SetActiveTarget(logTarget.get());
    }
    else
    {
        result.returnBody.messageEnum = UpdateResult::Enum::NoLogFile;
        CallAfter([this, result]() {
            API_UI_ReportUpdateResult(result);
            });
        return;
    }

    const long targetVersion = 2;
    const std::wstring oldExtensionName = L".old";
    std::vector<wxFileName> oldFiles;

    long installedVersion = GetInstalledVersion();
    while (installedVersion != targetVersion) {
        if (oldFiles.size() > 0) {
            wxLogMessage("Clean old files before update");
            for (auto& file : oldFiles)
            {
                if (file.Exists()) {
                    bool removeResult = fs::tRemoveFile(file);
                    if (removeResult == false) {
                        API_SRV_Update_OnRemoveFail(result, file);
                        return;
                    }
                }
            }
            oldFiles.clear();
        }

        if (installedVersion == 0) {
            wxLogMessage("Update from version 0 to 1");

            auto ini = rs::Program::GetIniDir();
            auto targetIni = ini;

            ini.SetName("1.ini");
            if (ini.Exists()) {
                targetIni.SetName("1.ini" + oldExtensionName);
                if (targetIni.Exists()) {
                    bool removeResult = fs::tRemoveFile(targetIni);
                    if (removeResult == false) {
                        API_SRV_Update_OnRemoveFail(result, targetIni);
                        return;
                    }
                }
                bool copyResult = fs::tCopyFile(ini, targetIni);
                if (copyResult == false) {
                    API_SRV_Update_OnCopyFail(result, ini, targetIni);
                    return;
                }
                oldFiles.push_back(targetIni);

                targetIni.SetName("1_.ini");
                bool renameResult = fs::tRenameFile(ini, targetIni);
                if (renameResult == false) {
                    API_SRV_Update_OnRenameFail(result, ini, targetIni);
                    return;
                }
            }

            ini.SetName("2.ini");
            if (ini.Exists()) {
                bool removeResult = fs::tRemoveFile(ini);
                if (removeResult == false) {
                    API_SRV_Update_OnRemoveFail(result, ini);
                    return;
                }
            }

            ss::updaterConfig->Write(sk::metadata_installedVersion, 1);
            ss::updaterConfig->Flush();
        }
        if (installedVersion == 1) {
            wxLogMessage("Update from version 1 to 2");

            auto ini = rs::Program::GetIniDir();
            auto targetIni = ini;

            ini.SetName("1_.ini");
            if (ini.Exists()) {
                targetIni.SetName("1_.ini" + oldExtensionName);
                if (targetIni.Exists()) {
                    bool removeResult = fs::tRemoveFile(targetIni);
                    if (removeResult == false) {
                        API_SRV_Update_OnRemoveFail(result, targetIni);
                        return;
                    }
                }
                bool copyResult = fs::tCopyFile(ini, targetIni);
                if (copyResult == false) {
                    API_SRV_Update_OnCopyFail(result, ini, targetIni);
                    return;
                }
                oldFiles.push_back(targetIni);

                targetIni.SetName("1_1.ini");
                bool renameResult = fs::tRenameFile(ini, targetIni);
                if (renameResult == false) {
                    API_SRV_Update_OnRenameFail(result, ini, targetIni);
                    return;
                }
            }

            ss::updaterConfig->Write(sk::metadata_installedVersion, 2);
            ss::updaterConfig->Flush();
        }

        installedVersion = GetInstalledVersion();
    }
    wxLogMessage("Update finished");

    auto joinString = String_SRV::JoinVectorNonStringObject<wxFileName>(oldFiles, ss::arrayDelimiter1.ToStdWstring(), &wxFileNameToString);
    if (joinString.success) {
        ss::updaterConfig->Write(sk::files_old, wxString(joinString.returnBody));
        ss::updaterConfig->Flush();
    }
    else {
        ss::updaterConfig->Write(sk::files_old, wxEmptyString);
        ss::updaterConfig->Flush();
    }

    result.success = true;
    CallAfter([this, result]() {
        wxLog::FlushActive();
        wxLog::SetActiveTarget(nullptr);
        Lock_SRV::Notify(uiMutex, uiCb);

        API_UI_ReportUpdateResult(result);
        });
    Lock_SRV::Wait(uiMutex, uiCb);
}
