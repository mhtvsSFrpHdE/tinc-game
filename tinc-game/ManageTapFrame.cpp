#include "ManageTapFrame.h"
#include "TapDevice_SRV.h"
#include "Layout_SRV.h"

ManageTapFrame::ManageTapFrame(MainFrame* parentFrame) : wxFrame(parentFrame, wxID_ANY, _("Manage virtual network adapter"))
{
    Init_CreateControls();
    Init_BindEventHandlers();
    Init_Layout();
}

void ManageTapFrame::Init_CreateControls()
{
    rootPanel = new wxPanel(this);
    defaultTap_StaticText = new wxStaticText(rootPanel, wxID_ANY, _("Current default virtual network adapter:"));
    defaultTapValue_TextCtrl = new wxTextCtrl(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    {
        auto getTap = TapDevice_SRV::GetDefaultTap();
        if (getTap.success) {
            defaultTapValue_TextCtrl->SetLabel(getTap.returnBody.adapter.friendlyName);
        }
        else {
            defaultTapValue_TextCtrl->SetLabel(defaultTapValue_NoneText);
        }
    }
}

void ManageTapFrame::Init_BindEventHandlers()
{
}

void ManageTapFrame::Init_Layout()
{
    namespace ls = Layout_SRV;

    //this->SetSizeHints(320, 270);

    wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
    rootPanel->SetSizer(rootSizer);
    ls::AddFixedSpacer(wxTOP, ls::SpaceToFrameBorder, rootSizer);

    rootSizer->Add(defaultTap_StaticText, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
    rootSizer->Add(defaultTapValue_TextCtrl, 0, wxLEFT, ls::SpaceToFrameBorder);
    ls::AddFixedSpacer(wxTOP, ls::SpaceBetweenControl, rootSizer);
}
