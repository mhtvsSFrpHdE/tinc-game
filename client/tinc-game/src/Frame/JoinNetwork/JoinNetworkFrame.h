#pragma once
#include <wx/wx.h>
#include "resource/ReturnValue.h"
#include <unordered_map>

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

enum class RegisterApiVersion {
    /// <summary>
    /// V1 in HTTP
    /// </summary>
    v1 = 1,
    /// <summary>
    /// V1 in HTTPS
    /// </summary>
    v1s = 2
};
extern std::unordered_map<wxString, RegisterApiVersion> apiVersionList;

class JoinNetworkFrame : public wxFrame
{
public:
    JoinNetworkFrame(wxFrame* parentFrame, std::function<void()> onCloseCallback);

private:
    wxFrame* _parentFrame;
    std::function<void()> _onCloseCallback;

    void API_SRV_JoinNetworkByInviteCode(std::wstring networkName, std::wstring inviteCode);
    void API_SRV_JoinNetworkByRegister(std::wstring networkName, RegisterApiVersion apiVersion, std::wstring wRegisterRequestUrl);

    void API_UI_ReportErrorMessage(ReturnValue<JoinNetworkResult> result);
    void API_UI_EndJoinNetworkByInviteCodeOnError();
    void API_UI_EndJoinNetworkByRegisterOnError();

    wxWindowDisabler makeModal;

    wxPanel* rootPanel;
    wxBoxSizer* rootSizer;

    wxStaticText* joinBy_StaticText;
    wxComboBox* joinBy_ComboBox;
    enum class JoinBy {
        Register = 0,
        InviteCode = 1
    };
    JoinBy joinBy = JoinBy::Register;
    void OnJoinByComboBoxSelect(wxCommandEvent& event);

    wxStaticText* saveAs_StaticText;
    wxComboBox* saveAs_ComboBox;

    wxBoxSizer* navigateSizer;
    wxButton* confirmButton;
    void OnConfirmButtonClick(wxCommandEvent& event);
    ReturnValue<wxString> OnConfirmButtonClick_GetNetworkName();
    void OnConfirmButtonClick_JoinByInviteCode();
    void OnConfirmButtonClick_JoinByInviteCode_UiEnable(bool enable);
    void OnConfirmButtonClick_JoinByRegister();
    void OnConfirmButtonClick_JoinByRegister_UiEnable(bool enable);
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

    wxPanel* retryPanel;

    wxStaticText* liveLog_StaticText;
    wxTextCtrl* liveLog_TextCtrl;


    void Init_CreateControls();
    void Init_Layout();

    bool allowCloseFrame = true;
    void OnClose(wxCloseEvent& event);
};
