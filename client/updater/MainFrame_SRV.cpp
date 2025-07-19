#include "MainFrame.h"
#include "Settings_SRV.h"
#include "../resource/Resource_SRV.h"
#include <sstream>
#include "../resource/String_SRV.h"
#include <fstream>

long GetInstalledVersion() {
    namespace ss = Settings_SRV;
    namespace sk = SettingKeys_Updater;
    namespace sdn = SettingDefaultValue_Updater;

    return ss::updaterConfig->ReadLong(sk::metadata_installedVersion, sdn::installedVersion);
}

std::string tSuccess(bool result) {
    if (result) {
        return std::string("Success");
    }
    else {
        return std::string("Fail");
    }
}

void tRemoveFile(wxFileName& file) {
    namespace ss = String_SRV;

    auto fullPath = file.GetFullPath();

    bool result = wxRemoveFile(fullPath);

    std::ostringstream logMessage;
    logMessage << "Remove" << ss::space
        << ss::doubleQuotes << fullPath << ss::doubleQuotes << ss::space
        << tSuccess(result);
    wxLogMessage(wxString(logMessage.str()));
}

void tCopyFile(wxFileName& srcfile, wxFileName& destfile) {
    namespace ss = String_SRV;

    auto srcFullPath = srcfile.GetFullPath();
    auto destFullPath = destfile.GetFullPath();

    bool result = wxCopyFile(srcFullPath, destFullPath);

    std::ostringstream logMessage;
    logMessage << "Copy" << ss::space
        << ss::doubleQuotes << srcFullPath << ss::doubleQuotes << ss::space
        << "to" << ss::space
        << ss::doubleQuotes << destFullPath << ss::doubleQuotes << ss::space
        << tSuccess(result);
    wxLogMessage(wxString(logMessage.str()));
}

void tRenameFile(wxFileName& srcfile, wxFileName& destfile) {
    namespace ss = String_SRV;

    auto srcFullPath = srcfile.GetFullPath();
    auto destFullPath = destfile.GetFullPath();

    bool result = wxRenameFile(srcFullPath, destFullPath);

    std::ostringstream logMessage;
    logMessage << "Rename" << ss::space
        << ss::doubleQuotes << srcFullPath << ss::doubleQuotes << ss::space
        << "to" << ss::space
        << ss::doubleQuotes << destFullPath << ss::doubleQuotes << ss::space
        << tSuccess(result);
    wxLogMessage(wxString(logMessage.str()));
}

std::wstring wxFileNameToString(wxFileName& file) {
    return file.GetName().ToStdWstring();
}

void MainFrame::API_SRV_PostLayout()
{
    namespace ss = Settings_SRV;
    namespace sk = SettingKeys_Updater;
    namespace sdn = SettingDefaultValue_Updater;
    namespace rs = Resource_SRV;

    std::ofstream outFile("application.log");
    std::shared_ptr<wxLogStream> logTarget;
    if (outFile.is_open())
    {
        logTarget = std::shared_ptr<wxLogStream>(new wxLogStream(&outFile));

        CallAfter([this, &logTarget]() {
            wxLog::SetActiveTarget(logTarget.get());
            Lock_SRV::Notify(uiMutex, uiCb);
            });
        Lock_SRV::Wait(uiMutex, uiCb);
    }
    else
    {
        CallAfter([this]() {
            wxMessageDialog(this, "Failed to open log file!").ShowModal();
            Close();
            });
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
                    tRemoveFile(file);
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
                    tRemoveFile(targetIni);
                }
                tCopyFile(ini, targetIni);
                oldFiles.push_back(targetIni);

                targetIni.SetName("1_.ini");
                tRenameFile(ini, targetIni);
            }

            ini.SetName("2.ini");
            if (ini.Exists()) {
                tRemoveFile(ini);
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
                    tRemoveFile(targetIni);
                }
                tCopyFile(ini, targetIni);
                oldFiles.push_back(targetIni);

                targetIni.SetName("1_1.ini");
                tRenameFile(ini, targetIni);
            }

            ss::updaterConfig->Write(sk::metadata_installedVersion, 2);
            ss::updaterConfig->Flush();
        }

        installedVersion = GetInstalledVersion();
    }

    auto joinString = String_SRV::JoinVectorNonStringObject<wxFileName>(oldFiles, ss::arrayDelimiter1.ToStdWstring(), &wxFileNameToString);
    if (joinString.success) {
        ss::updaterConfig->Write(sk::files_old, wxString(joinString.returnBody));
        ss::updaterConfig->Flush();
    }
    else {
        ss::updaterConfig->Write(sk::files_old, wxEmptyString);
        ss::updaterConfig->Flush();
    }

    CallAfter([this]() {
        wxLog::SetActiveTarget(nullptr);
        Lock_SRV::Notify(uiMutex, uiCb);

        Enable(true);
        Raise();
        Close();
        });
    Lock_SRV::Wait(uiMutex, uiCb);
}
