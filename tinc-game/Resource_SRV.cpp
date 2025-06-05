#include "Resource_SRV.h"
#include "String_SRV.h"
#include <sstream>
#include <wx/filename.h>

void IntegrityCheck_DirExist(std::wostringstream& helpText, wxFileName& file, int& failedCount) {
    namespace ss = String_SRV;

    bool exists = file.DirExists();
    helpText << file.GetFullPath() << ss::ellipses
        << (exists ? _("OK") : _("Fail"))
        << ss::newLine;
    if (exists == false) {
        failedCount++;
    }
}

void IntegrityCheck_FileExist(std::wostringstream& helpText, wxFileName& file, int& failedCount) {
    namespace ss = String_SRV;

    bool exists = file.Exists();
    helpText << file.GetFullPath() << ss::ellipses
        << (exists ? _("OK") : _("Fail"))
        << ss::newLine;
    if (exists == false) {
        failedCount++;
    }
}

std::wstring Resource_SRV::IntegrityCheck()
{
    int failedCount = 0;

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
        IntegrityCheck_FileExist(helpText, file, failedCount);
        file.SetName(L"OemWin2k.inf");
        IntegrityCheck_FileExist(helpText, file, failedCount);
        file.SetName(L"tap0901.cat");
        IntegrityCheck_FileExist(helpText, file, failedCount);
        file.SetName(L"tap0901.sys");
        IntegrityCheck_FileExist(helpText, file, failedCount);

        helpText << ss::newLine;
        file.Clear();
    }

    {
        helpText << _("Command line I/O files:") << ss::newLine;

        file.SetName(L"getTapHwid.bat");
        IntegrityCheck_FileExist(helpText, file, failedCount);
        file.SetName(L"installTap.bat");
        IntegrityCheck_FileExist(helpText, file, failedCount);
        file.SetName(L"uninstallTap.bat");
        IntegrityCheck_FileExist(helpText, file, failedCount);
        file.SetName(L"netsh437.bat");
        IntegrityCheck_FileExist(helpText, file, failedCount);
        file.SetName(L"netsh437_start.bat");
        IntegrityCheck_FileExist(helpText, file, failedCount);
        file.SetName(L"ping437.bat");
        IntegrityCheck_FileExist(helpText, file, failedCount);
        file.SetName(L"ping437_start.bat");
        IntegrityCheck_FileExist(helpText, file, failedCount);

        helpText << ss::newLine;
        file.Clear();
    }

    {
        helpText << _("Translation files:") << ss::newLine;

        file.AppendDir(L"zh");
        IntegrityCheck_DirExist(helpText, file, failedCount);

        helpText << ss::newLine;
        file.Clear();
    }

    if (failedCount >= 4) {
        helpText << _("Failed items more than 4, don't double click tinc-game in zip/7z/rar files directly you foo")
            << ss::newLine << ss::newLine;
    }

    auto collectHelpText = helpText.str();
    return collectHelpText;
}
