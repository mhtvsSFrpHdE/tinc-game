#pragma once
#include "wx/filename.h"
#include "../resource/String_SRV.h"
#include <sstream>
#include "wx/log.h"

namespace File_SRV
{
    std::string tSuccess(bool result);

    bool tCopyFile(wxFileName& srcfile, wxFileName& destfile);

    /// <summary>
    /// If dest file exist, will be overwrite
    /// </summary>
    /// <param name="srcfile"></param>
    /// <param name="destfile"></param>
    /// <returns></returns>
    bool tRenameFile(wxFileName& srcfile, wxFileName& destfile);

    bool tRemoveFile(wxFileName& file);
};
