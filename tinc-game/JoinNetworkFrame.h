#pragma once
#include <wx/wx.h>

class JoinNetworkFrame : public wxFrame
{
public:
    JoinNetworkFrame(wxFrame* parentFrame);

private:
    wxWindowDisabler makeModal;

    wxPanel* rootPanel = nullptr;
    wxPanel* joinByRegisterPanel = nullptr;
    wxPanel* joinByInviteCodePanel = nullptr;
    wxPanel* joinByImportPanel = nullptr;

    wxStaticText* joinBy_StaticText = nullptr;
    wxComboBox* joinBy_ComboBox = nullptr;
    enum class JoinBy {
        InviteCode = 0
    };
    JoinBy joinBy = JoinBy::InviteCode;

    wxStaticText* inviteCode_StaticText = nullptr;
    wxTextCtrl* inviteCode_TextCtrl = nullptr;
    wxStaticText* saveAs_StaticText = nullptr;
    wxComboBox* saveAs_ComboBox = nullptr;

    wxButton* joinButton = nullptr;
    void OnJoinButtonClick(wxCommandEvent& event);
    wxButton* closeButton = nullptr;
    void OnCloseButtonClick(wxCommandEvent& event);

    void Init_CreateControls();
    void Init_Layout();
};

