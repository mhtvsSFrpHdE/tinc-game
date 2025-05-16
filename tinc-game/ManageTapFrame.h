#pragma once
#include <wx/wx.h>
#include "MainFrame.h"

class ManageTapFrame : public wxFrame
{
public:
    ManageTapFrame(MainFrame* parentFrame);

private:
    wxWindowDisabler makeModal;

    wxPanel* rootPanel = nullptr;

    wxStaticText* defaultTap_StaticText = nullptr;
    wxTextCtrl* defaultTapValue_TextCtrl = nullptr;
    wxString defaultTapValue_NoneText = _("None");

    void Init_CreateControls();
    void Init_BindEventHandlers();
    void Init_Layout();
};
