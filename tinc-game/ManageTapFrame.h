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
    Enum messageEnum;
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
    Enum messageEnum;
    std::wstring messageString;
};

class ManageTapFrame : public wxFrame
{
public:
    ManageTapFrame(wxFrame* parentFrame);

    // UI to SRV
    static ReturnValue<InstallTapResult> API_SRV_InstallTap();
    static ReturnValue<UninstallTapResult> API_SRV_UninstallTap(WindowsAPI_SRV::GetAdaptersAddressesResult adapter);

    void OnHelpFrameOpenDeviceManagerButtonClick(wxCommandEvent& evt);

private:
    wxWindowDisabler makeModal;

    wxPanel* rootPanel = nullptr;

    wxButton* helpButton = nullptr;
    wxStaticText* defaultTap_StaticText = nullptr;
    wxTextCtrl* defaultTapValue_TextCtrl = nullptr;
    wxString defaultTapValue_NoneText = _("None");
    bool hasDefaultTap = false;
    bool suggestReadHelp = true;

    wxStaticText* manageTap_StaticText = nullptr;
    wxButton* setAsDefault_Button = nullptr;
    wxButton* installTap_Button = nullptr;
    wxButton* uninstallTapButton = nullptr;

    wxComboBox* installedTap_ComboBox = nullptr;
    std::unordered_map<int, WindowsAPI_SRV::GetAdaptersAddressesResult> installedTap_ComboBox_RawData;
    wxButton* openDeviceManagerButton = nullptr;
    wxButton* installedTapHelpMeDecide_Button = nullptr;
    wxButton* installedTapRefresh_Button = nullptr;

    wxButton* closeButton = nullptr;

    void Init_CreateControls();
    void Init_Layout();

    void OnClose(wxCloseEvent& event);
    void OnHelpButtonClick_Internal();
    void OnHelpButtonClick(wxCommandEvent& evt);
    void OnHelpFrameCloseCallback();
    void OnInstalledTapComboBoxChange(wxCommandEvent& evt);
    void OnInstalledTapHelpMeDecideButtonClick(wxCommandEvent& evt);
    void OnInstalledTapRefreshButtonClick(wxCommandEvent& evt);
    void OnSetAsDefaultButtonClick(wxCommandEvent& evt);
    void OnInstallTapButtonClick(wxCommandEvent& evt);
    void OnUninstallTapButtonClick(wxCommandEvent& evt);
    void OnCloseButtonClick(wxCommandEvent& evt);

    void Reload_installedTap_ComboBox();
    void Reload_defaultTapValue_TextCtrl();
    void Reload();

    bool allowCloseFrame = true;
};
