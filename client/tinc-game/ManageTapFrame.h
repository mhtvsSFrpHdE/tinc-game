#pragma once
#include <wx/wx.h>
#include <unordered_map>
#include "ReturnValue.h"
#include "WindowsAPI_SRV.h"
#include <string>

struct InstallTapResult {
    enum class Enum {
        DriverNotExist,
        UserCanceled,
        InstallerNotExist,
        NoPermission,
        Other
    };
    Enum messageEnum = Enum::Other;
    std::wstring messageString;
};

struct UninstallTapResult {
    enum class Enum {
        NoAnyAdapter,
        NoPermission,
        NotFound,
        UninstallerNotExist,
        Other
    };
    Enum messageEnum = Enum::Other;
    std::wstring messageString;
};

class ManageTapFrame : public wxFrame
{
public:
    ManageTapFrame(wxFrame* parentFrame, std::function<void()> onCloseCallback);

    static ReturnValue<InstallTapResult> API_SRV_InstallTap();
    static ReturnValue<UninstallTapResult> API_SRV_UninstallTap(WindowsAPI_SRV::GetAdaptersAddressesResult adapter);

    void OnHelpFrameOpenDeviceManagerButtonClick(wxCommandEvent& evt);

private:
    wxFrame* _parentFrame;
    std::function<void()> _onCloseCallback;

    wxWindowDisabler makeModal;

    wxPanel* rootPanel;

    wxButton* helpButton;
    wxStaticText* firstTap_StaticText;
    wxTextCtrl* firstTapValue_TextCtrl;
    wxString firstTapValue_NoneText = _("None");
    bool hasFirstTap = false;

    bool suggestReadHelp = true;

    wxStaticText* manageTap_StaticText;
    wxButton* installTap_Button;
    wxButton* setMetric_Button;
    wxButton* uninstallTapButton;

    wxComboBox* installedTap_ComboBox;
    std::unordered_map<int, WindowsAPI_SRV::GetAdaptersAddressesResult> installedTap_ComboBox_RawData;
    wxButton* openDeviceManagerButton;
    wxButton* installedTapRename_Button;
    wxButton* installedTapRefresh_Button;

    wxButton* closeButton;

    void Init_CreateControls();
    void Init_Layout();

    void OnClose(wxCloseEvent& event);
    void OnHelpButtonClick_Internal();
    void OnHelpButtonClick(wxCommandEvent& evt);
    void OnHelpFrameCloseCallback();
    void OnInstalledTapComboBoxChange(wxCommandEvent& evt);
    void OnInstalledTapRenameButtonClick(wxCommandEvent& evt);
    void OnInstalledTapRefreshButtonClick(wxCommandEvent& evt);
    void OnSetMetricButtonclick(wxCommandEvent& evt);
    void OnInstallTapButtonClick(wxCommandEvent& evt);
    void OnUninstallTapButtonClick(wxCommandEvent& evt);
    void OnCloseButtonClick(wxCommandEvent& evt);

    void Reload_installedTap_ComboBox();
    void Reload_firstTapValue_TextCtrl();
    void Reload();
};
