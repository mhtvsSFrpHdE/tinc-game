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
    JoinNetworkFrame(wxFrame* parentFrame, std::function<void()> onCloseCallback);

private:
    wxFrame* _parentFrame;
    std::function<void()> _onCloseCallback;

    void API_SRV_JoinNetworkByInviteCode(std::wstring networkName, std::wstring inviteCode);

    void API_UI_EndJoinNetworkByInviteCode(ReturnValue<JoinNetworkResult> result);

    wxWindowDisabler makeModal;

    wxPanel* rootPanel;
    wxBoxSizer* rootSizer;

    wxStaticText* joinBy_StaticText;
    wxComboBox* joinBy_ComboBox;
    enum class JoinBy {
        InviteCode = 0
    };
    JoinBy joinBy = JoinBy::InviteCode;

    wxStaticText* saveAs_StaticText;
    wxComboBox* saveAs_ComboBox;

    wxBoxSizer* navigateSizer;
    wxButton* confirmButton;
    void OnConfirmButtonClick(wxCommandEvent& event);
    wxButton* retryButton;
    void OnRetryButtonClick(wxCommandEvent& event);
    wxButton* cancelButton;
    void OnCancelButtonClick(wxCommandEvent& event);

    wxPanel* joinByInviteCodePanel;

    wxStaticText* inviteCode_StaticText;
    wxTextCtrl* inviteCode_TextCtrl;
    void OnInviteCodeChanged(wxCommandEvent& event);

    wxPanel* retryPanel;

    wxStaticText* liveLog_StaticText;
    wxTextCtrl* liveLog_TextCtrl;

    wxPanel* joinByRegisterPanel;

    void Init_CreateControls();
    void Init_Layout();

    bool allowCloseFrame = true;
    void OnClose(wxCloseEvent& event);
};
