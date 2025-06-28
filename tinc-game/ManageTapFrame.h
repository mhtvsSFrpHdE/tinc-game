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

    // UI to SRV
    static ReturnValue<InstallTapResult> API_SRV_InstallTap();
    static ReturnValue<UninstallTapResult> API_SRV_UninstallTap(WindowsAPI_SRV::GetAdaptersAddressesResult adapter);

    void OnHelpFrameOpenDeviceManagerButtonClick(wxCommandEvent& evt);

private:
    wxFrame* _parentFrame;
    std::function<void()> _onCloseCallback;

    wxWindowDisabler makeModal;

    wxPanel* rootPanel = nullptr;

    wxButton* helpButton = nullptr;
    wxStaticText* firstTap_StaticText = nullptr;
    wxTextCtrl* firstTapValue_TextCtrl = nullptr;
    wxString firstTapValue_NoneText = _("None");
    bool hasFirstTap = false;

    bool suggestReadHelp = true;

    wxStaticText* manageTap_StaticText = nullptr;
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
    void OnInstallTapButtonClick(wxCommandEvent& evt);
    void OnUninstallTapButtonClick(wxCommandEvent& evt);
    void OnCloseButtonClick(wxCommandEvent& evt);

    void Reload_installedTap_ComboBox();
    void Reload_firstTapValue_TextCtrl();
    void Reload();

    bool allowCloseFrame = true;
};
