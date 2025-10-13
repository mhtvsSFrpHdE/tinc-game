#pragma once
#include <wx/filename.h>

namespace Resource_SRV
{
    namespace Program {
        wxFileName GetProgramDir();
        wxFileName GetIniDir();
        const wxString binDir = wxT("bin");
        wxFileName GetBinDir();
        const wxString programExe = wxT("tinc-game.exe");
        const wxString programIni = wxT("tinc-game.ini");
        const wxString networksIni = wxT("tinc-game-networks.ini");
        const wxString updaterIni = wxT("updater.ini");
        const std::wstring updaterLog = L"tinc-game-updater.log";
    }
}
