#pragma once
#include <wx/wx.h>
#include "MainFrame.h"
#include <unordered_map>
#include "ReturnValue.h"
#include "WindowsAPI_SRV.h"
#include <string>

class ManageTapFrame : public wxFrame
{
public:
    ManageTapFrame(MainFrame* parentFrame);

    // UI to SRV
    static ReturnValue<std::vector<WindowsAPI_SRV::GetAdaptersAddressesResult>> API_SRV_GetNetworkAdapterList();
    static ReturnValue<std::wstring> API_SRV_InstallTap();
    static ReturnValue<std::wstring> API_SRV_UninstallTap(WindowsAPI_SRV::GetAdaptersAddressesResult adapter);

private:
    wxWindowDisabler makeModal;

    wxPanel* rootPanel = nullptr;

    wxStaticText* defaultTap_StaticText = nullptr;
    wxTextCtrl* defaultTapValue_TextCtrl = nullptr;
    wxString defaultTapValue_NoneText = _("None");
    bool hasDefaultTap = false;

    wxStaticText* manageTap_StaticText = nullptr;
    wxButton* installTap_Button = nullptr;

    wxComboBox* installedTap_ComboBox = nullptr;
    std::unordered_map<int, WindowsAPI_SRV::GetAdaptersAddressesResult> installedTap_ComboBox_RawData;
    wxButton* uninstallTapButton = nullptr;

    wxButton* closeButton = nullptr;

    void Init_CreateControls();
    void Init_BindEventHandlers();
    void Init_Layout();

    void OnClose(wxCloseEvent& event);
    void OnInstalledTapComboBoxChange(wxCommandEvent& evt);
    void OnInstallTapButtonClick(wxCommandEvent& evt);
    void OnUninstallTapButtonClick(wxCommandEvent& evt);
    void OnCloseButtonClick(wxCommandEvent& evt);

    void Reload_installedTap_ComboBox();
    void Reload_defaultTapValue_TextCtrl();
    void Reload();

    bool allowCloseFrame = true;
};
