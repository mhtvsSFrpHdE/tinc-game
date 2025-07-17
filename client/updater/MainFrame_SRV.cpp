#include "MainFrame.h"
#include "Settings_SRV.h"
#include "../resource/Resource_SRV.h"

void MainFrame::API_SRV_PostLayout()
{
    namespace ss = Settings_SRV;
    namespace sk = SettingKeys_Updater;
    namespace sdn = SettingDefaultValue_Updater;
    namespace rs = Resource_SRV;

    const long targetVersion = 2;

    long installedVersion = 0;
    while (installedVersion != targetVersion) {
        installedVersion = ss::updaterConfig->ReadLong(sk::metadata_installedVersion, sdn::installedVersion);
        if (installedVersion == 0) {
            auto ini = rs::Program::GetIniDir();
            auto targetIni = ini;
            ini.SetName("1.ini");
            if (ini.Exists()) {
                targetIni.SetName("1_.ini");
                wxRenameFile(ini.GetFullPath(), targetIni.GetFullPath());
            }
            ini.SetName("2.ini");
            if (ini.Exists()) {
                wxRemoveFile(ini.GetFullPath());
            }
            ss::updaterConfig->Write(sk::metadata_installedVersion, 1);
            ss::updaterConfig->Flush();
        }
        if (installedVersion == 1) {
            auto ini = rs::Program::GetIniDir();
            auto targetIni = ini;
            ini.SetName("1_.ini");
            if (ini.Exists()) {
                targetIni.SetName("1_1.ini");
                wxRenameFile(ini.GetFullPath(), targetIni.GetFullPath());
            }
            ss::updaterConfig->Write(sk::metadata_installedVersion, 2);
            ss::updaterConfig->Flush();
        }
    }

    CallAfter([this]() {
        Enable(true);
        Raise();
        });
}
