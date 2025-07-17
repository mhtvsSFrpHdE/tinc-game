#include "pch.h"
#include "Resource_SRV.h"
#include "String_SRV.h"
#include <sstream>
#include "boost/optional.hpp"
#include <wx/stdpaths.h>

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

boost::optional<wxString> nullableForbiddenChars;
ReturnValue<wxString> Resource_SRV::IsValidFileOrDir(wxString& name)
{
    auto result = ReturnValue<wxString>();

    if (!nullableForbiddenChars) {
        nullableForbiddenChars = wxFileName::GetForbiddenChars() + "/\\";
    }

    auto& forbiddenChars = nullableForbiddenChars.get();
    for (wxChar wxC : forbiddenChars) {
        if (name.Contains(wxC)) {
            result.returnBody = wxC;
            return result;
        };
    }

    result.success = true;
    return result;
}

boost::optional<wxFileName> nullableTapInstallerDir;
wxFileName Resource_SRV::TincBin::GetTapInstallerDir()
{
    if (nullableTapInstallerDir) {
        return nullableTapInstallerDir.get();
    }

    wxFileName file;
    file.AppendDir(binDir);
    file.AppendDir(tincBinDir);
    file.AppendDir(tapInstallerDir);

    nullableTapInstallerDir = file;
    return file;
}

boost::optional<wxFileName> nullableTincBinDir;
wxFileName Resource_SRV::TincBin::GetTincBinDir()
{
    if (nullableTincBinDir) {
        return nullableTincBinDir.get();
    }

    wxFileName file;
    file.AppendDir(binDir);
    file.AppendDir(tincBinDir);

    nullableTincBinDir = file;
    return file;
}

boost::optional<wxString> nullableTincBinPath;
wxString Resource_SRV::TincBin::GetTincBinAsWxStr()
{
    if (nullableTincBinPath) {
        return nullableTincBinPath.get();
    }

    wxFileName file;
    file.AppendDir(binDir);
    file.AppendDir(tincBinDir);
    file.SetName(tincBin);

    nullableTincBinPath = file.GetFullPath();
    return nullableTincBinPath.get();
}

boost::optional<wxString> nullableTincdBinPath;
wxString Resource_SRV::TincBin::GetTincdBinAsWxStr()
{
    if (nullableTincdBinPath) {
        return nullableTincdBinPath.get();
    }

    wxFileName file;
    file.AppendDir(binDir);
    file.AppendDir(tincBinDir);
    file.SetName(tincdBin);

    nullableTincdBinPath = file.GetFullPath();
    return nullableTincdBinPath.get();
}

boost::optional<wxFileName> nullableTincdBin;
wxFileName Resource_SRV::TincBin::GetTincdBin()
{
    if (nullableTincdBin) {
        return nullableTincdBin.get();
    }

    wxFileName file;
    file.AppendDir(binDir);
    file.AppendDir(tincBinDir);
    file.SetName(tincdBin);

    nullableTincdBin = file;
    return nullableTincdBin.get();
}

wxFileName Resource_SRV::TincBin::GetTincdGameModeBin(wxString gameModeValue)
{
    wxFileName file;
    file.AppendDir(binDir);
    file.AppendDir(tincBinDir);
    file.SetName(gameModeValue);

    return file;
}

boost::optional<wxFileName> nullableTincdPid;
wxFileName Resource_SRV::TincBin::GetTincdPid(std::wstring networkFullPath)
{
    if (nullableTincdPid) {
        return nullableTincdPid.get();
    }

    wxFileName file;
    file.SetPath(networkFullPath);
    file.SetName(tincdPid);

    nullableTincdPid = file;
    return file;
}

std::unique_ptr<wxDir> Resource_SRV::Networks::GetNetworksDirAsWxDir()
{
    auto dir = std::unique_ptr<wxDir>(new wxDir(networksDir));
    return dir;
}

boost::optional<wxFileName> nullableNetworksDir;
wxFileName Resource_SRV::Networks::GetNetworksDir()
{
    if (nullableNetworksDir) {
        return nullableNetworksDir.get();
    }

    wxFileName file;
    file.AppendDir(networksDir);

    nullableNetworksDir = file;
    return file;
}

boost::optional<wxFileName> nullableIniDir;
wxFileName Resource_SRV::Program::GetIniDir()
{
    if (nullableIniDir) {
        return nullableIniDir.get();
    }

    auto exePath = wxStandardPaths::Get().GetExecutablePath();
    wxFileName file(exePath);
    file.ClearExt();
    file.SetName(wxEmptyString);

    nullableIniDir = file;
    return file;
}

void Resource_SRV::wxWidgets::wxButtonDeleter(wxButton* obj)
{
    obj->Destroy();
}

void Resource_SRV::wxWidgets::wxTextCtrlDeleter(wxTextCtrl* obj)
{
    obj->Destroy();
}
