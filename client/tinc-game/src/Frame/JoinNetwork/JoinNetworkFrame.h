#pragma once
#include <wx/wx.h>
#include "resource/ReturnValue.h"

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

    void API_UI_ReportErrorMessage(ReturnValue<JoinNetworkResult> result);
    void API_UI_EndJoinNetworkByInviteCode(ReturnValue<JoinNetworkResult> result);

    wxWindowDisabler makeModal;

    wxPanel* rootPanel;
    wxBoxSizer* rootSizer;

    wxStaticText* joinBy_StaticText;
    wxComboBox* joinBy_ComboBox;
    enum class JoinBy {
        Register = 0,
        InviteCode = 1
    };
    JoinBy joinBy = JoinBy::InviteCode;
    void OnJoinByComboBoxSelect(wxCommandEvent& event);

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

    wxPanel* joinByRegisterPanel;

    wxStaticText* serverAddressAndPort_StaticText;
    wxComboBox* serverAddressAndPort_ComboBox;
    void OnServerAddressAndPortChanged(wxCommandEvent& event);

    wxPanel* retryPanel;

    wxStaticText* liveLog_StaticText;
    wxTextCtrl* liveLog_TextCtrl;


    void Init_CreateControls();
    void Init_Layout();

    bool allowCloseFrame = true;
    void OnClose(wxCloseEvent& event);
};
