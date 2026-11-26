#include "pch.h"
#include "Resource_SRV_TincBin.h"

#if defined(_WIN64)
const wxString Resource_SRV::TincBin::tapInstallerDir = wxT("tap-win64");
#else
const wxString Resource_SRV::TincBin::tapInstallerDir = wxT("tap-win32");
#endif
