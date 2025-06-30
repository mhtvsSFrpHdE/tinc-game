#include "HelpFrame.h"
#include "Layout_SRV.h"

HelpFrame::HelpFrame(wxFrame* parentFrame, wxString title, std::function<void()> onCloseCallback, bool defaultInit) : wxFrame(parentFrame, wxID_ANY, title)
{
    _parentFrame = parentFrame;
    _onCloseCallback = onCloseCallback;

    Init_CreateControls(defaultInit);
    Init_Layout(defaultInit);
}

void HelpFrame::SetHelpText(wxString helpText)
{
    helpText_TextCtrl->ChangeValue(helpText);
}

void HelpFrame::Init_CreateControls(bool defaultInit)
{
    rootPanel = new wxPanel(this);

    if (defaultInit) {
        helpText_TextCtrl = new wxTextCtrl(rootPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);
    }

    Bind(wxEVT_CLOSE_WINDOW, &HelpFrame::OnClose, this);
}

void HelpFrame::Init_Layout(bool defaultInit)
{
    namespace ls = Layout_SRV;

    this->SetSizeHints(640, 480);

    rootSizer = new wxBoxSizer(wxVERTICAL);
    rootPanel->SetSizer(rootSizer);

    if (defaultInit) {
        rootSizer->Add(helpText_TextCtrl, 1, wxEXPAND | wxALL, ls::SpaceToFrameBorder);
    }

    this->Fit();
}

void HelpFrame::OnClose(wxCloseEvent& event)
{
    _onCloseCallback();
    _parentFrame->Raise();

    event.Skip();
}
