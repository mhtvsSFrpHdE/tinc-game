#pragma once
#include <wx/dir.h>
#include <memory>
#include <wx/filename.h>

namespace Resource_SRV
{
    namespace Networks {
        const wxString networksDir = wxT("networks");
        std::unique_ptr<wxDir> GetNetworksDirAsWxDir();
        wxFileName GetNetworksDir();
    }
}
