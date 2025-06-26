#include "MainFrame.h"

void MainFrame::OnMenuNetworksEdit(wxCommandEvent& event)
{
    wxMessageDialog(this, _("Edit")).ShowModal();
}

void MainFrame::OnMenuNetworksJoin(wxCommandEvent& event)
{
    wxMessageDialog(this, _("Join network")).ShowModal();
}

void MainFrame::OnMenuNetworksRename(wxCommandEvent& event)
{
    wxMessageDialog(this, _("Rename")).ShowModal();
}

void MainFrame::OnMenuNetworksAdvancedDelete(wxCommandEvent& event)
{
    wxMessageDialog(this, _("Delete")).ShowModal();
}
