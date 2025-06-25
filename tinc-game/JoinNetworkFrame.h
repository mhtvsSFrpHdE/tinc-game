#pragma once
#include <wx/wx.h>
#include "ReturnValue.h"

struct JoinNetworkResult {
    enum class Enum {
        Other,
        InvalidInviteCode,
        ConnectFailed,
        AuthenticateFailed,
        TincBinFailed
    };
    Enum messageEnum = Enum::Other;
    std::wstring messageString;
};

class JoinNetworkFrame : public wxFrame
{
public:
    JoinNetworkFrame(wxFrame* parentFrame);

private:
    // UI to SRV
    void API_SRV_JoinNetworkByInviteCode(std::wstring networkName, std::wstring inviteCode);

    // SRV to UI
    void API_UI_EndJoinNetworkByInviteCode(ReturnValue<JoinNetworkResult> result);

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

    wxStaticText* saveAs_StaticText = nullptr;
    wxComboBox* saveAs_ComboBox = nullptr;

    wxStaticText* inviteCode_StaticText = nullptr;
    wxTextCtrl* inviteCode_TextCtrl = nullptr;
    void OnInviteCodeChanged(wxCommandEvent& event);

    wxButton* joinButton = nullptr;
    void OnJoinButtonClick(wxCommandEvent& event);
    wxButton* closeButton = nullptr;
    void OnCloseButtonClick(wxCommandEvent& event);

    void Init_CreateControls();
    void Init_Layout();
};
