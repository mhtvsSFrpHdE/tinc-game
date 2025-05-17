#pragma once
#include <wx/wx.h>
#include "MainFrame.h"
#include <unordered_map>
#include "ReturnValue.h"
#include "WindowsAPI_SRV.h"

class ManageTapFrame : public wxFrame
{
public:
    ManageTapFrame(MainFrame* parentFrame);

    // UI to SRV
    static ReturnValue<std::vector<WindowsAPI_SRV::GetAdaptersAddressesResult>> API_SRV_GetNetworkAdapterList();
    //void API_SRV_InstallTap();

private:
    wxWindowDisabler makeModal;

    wxPanel* rootPanel = nullptr;

    wxStaticText* defaultTap_StaticText = nullptr;
    wxTextCtrl* defaultTapValue_TextCtrl = nullptr;
    wxString defaultTapValue_NoneText = _("None");
    bool haveDefaultTap = false;

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
    void OnCloseButtonClick(wxCommandEvent& evt);

    bool allowCloseFrame = true;
};
