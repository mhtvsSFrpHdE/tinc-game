#pragma once
#include "wx/filename.h"
#include "../resource/String_SRV.h"
#include <sstream>
#include "wx/log.h"

/// <summary>
/// Function begin with "t" stands for "tinc"
/// Means this function is designed for tinc project
/// has non standard behavior usually bundle with GUI stuff
/// </summary>
namespace FileSystem_SRV
{
    std::string tActionResult(bool result);

    bool tCopyFile(wxFileName& srcfile, wxFileName& destfile);

    /// <summary>
    /// If dest file exist, will be overwrite
    /// </summary>
    /// <param name="srcfile"></param>
    /// <param name="destfile"></param>
    /// <returns></returns>
    bool tRenameFile(wxFileName& srcfile, wxFileName& destfile);

    bool tRemoveFile(wxFileName& file);

    bool tRemoveDir(wxFileName& dir);
};
