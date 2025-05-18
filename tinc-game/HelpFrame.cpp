#include "HelpFrame.h"
#include "Layout_SRV.h"
#include "OptimizeMtuFrame.h"

HelpFrame::HelpFrame(OptimizeMtuFrame* parentFrame, wxString title) : wxFrame(parentFrame, wxID_ANY, title)
{
    _parentFrame = parentFrame;

    Init_CreateControls();
    Init_Layout();
}

void HelpFrame::SetHelpText(wxString helpText)
{
    helpText_TextCtrl->ChangeValue(helpText);
}

void HelpFrame::Init_CreateControls()
{
    rootPanel = new wxPanel(this);
    helpText_TextCtrl = new wxTextCtrl(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);

    Bind(wxEVT_CLOSE_WINDOW, &HelpFrame::OnClose, this);
}

void HelpFrame::Init_Layout()
{
    namespace ls = Layout_SRV;

    this->SetSizeHints(640, 480);

    wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
    rootPanel->SetSizer(rootSizer);
    rootSizer->Add(helpText_TextCtrl, 1, wxEXPAND | wxALL, ls::SpaceToFrameBorder);
}

void HelpFrame::OnClose(wxCloseEvent& event)
{
    _parentFrame->helpButton->Enable(true);

    event.Skip();
}
