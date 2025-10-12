#include "File_SRV.h"

std::string File_SRV::tSuccess(bool result) {
    if (result) {
        return std::string("Success");
    }
    else {
        return std::string("Fail");
    }
}

bool File_SRV::tCopyFile(wxFileName& srcfile, wxFileName& destfile) {
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

    return result;
}

/// <summary>
/// If dest file exist, will be overwrite
/// </summary>
/// <param name="srcfile"></param>
/// <param name="destfile"></param>
/// <returns></returns>
bool File_SRV::tRenameFile(wxFileName& srcfile, wxFileName& destfile) {
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

    return result;
}

bool File_SRV::tRemoveFile(wxFileName& file) {
    namespace ss = String_SRV;

    auto fullPath = file.GetFullPath();

    bool result = wxRemoveFile(fullPath);

    std::ostringstream logMessage;
    logMessage << "Remove" << ss::space
        << ss::doubleQuotes << fullPath << ss::doubleQuotes << ss::space
        << tSuccess(result);
    wxLogMessage(wxString(logMessage.str()));

    return result;
}
