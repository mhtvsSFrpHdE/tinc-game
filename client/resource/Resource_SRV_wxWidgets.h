#pragma once
#include <wx/wx.h>

namespace Resource_SRV
{
    namespace wxWidgets {
        void wxButtonDeleter(wxButton* obj);
        void wxTextCtrlDeleter(wxTextCtrl* obj);
    }
}
