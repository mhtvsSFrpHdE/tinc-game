#pragma once
#include <wx/wx.h>
#include <unordered_map>
#include "TapDevice_SRV.h"
#include "Networks_SRV.h"

struct ConnectToNetworkResult {
    enum class Enum {
        TapUnavailable,
        RefusedByTinc,
        TincdNotExist,
        Other
    };
    Enum messageEnum = Enum::Other;
    std::wstring messageString;
    Networks_SRV::GetNetworksResult network;
    WindowsAPI_SRV::GetAdaptersAddressesResult tap;
};

class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    // UI to SRV
    void API_SRV_ConnectToNetwork(Networks_SRV::GetNetworksResult network, WindowsAPI_SRV::GetAdaptersAddressesResult tap);

    // SRV to UI
    void API_UI_ReportStatus(std::wstring status, wxTextCtrl* liveLog);
    void API_UI_EndConnectToNetwork(ReturnValue<ConnectToNetworkResult> result);

    wxPanel* rootPanel;

    wxStaticText* currentNetwork_StaticText = nullptr;
    wxComboBox* currentNetwork_ComboBox = nullptr;
    std::unordered_map<int, Networks_SRV::GetNetworksResult> currentNetwork_ComboBox_RawData;
    void OnCurrentNetworkChange(wxCommandEvent& evt);

    wxStaticText* currentTap_StaticText = nullptr;
    wxComboBox* currentTap_ComboBox = nullptr;
    std::unordered_map<int, WindowsAPI_SRV::GetAdaptersAddressesResult> currentTap_ComboBox_RawData;

    wxButton* connectButton = nullptr;
    void OnConnectButtonClick(wxCommandEvent& evt);
    wxButton* disconnectButton = nullptr;
    void OnDisconnectButtonClick(wxCommandEvent& evt);

    wxTextCtrl* liveLog = nullptr;

    wxButton* optimizeMtuButton = nullptr;
    void OnOptimizeMtuButton(wxCommandEvent& evt);
    void OnOptimizeMtuFrameCloseCallback();
    wxButton* manageTapDeviceButton = nullptr;
    void OnManageTapButton(wxCommandEvent& evt);
    wxButton* integrityCheckButton = nullptr;
    void OnIntegrityCheckButton(wxCommandEvent& evt);
    void OnIntegrityCheckFrameCloseCallback();
    wxButton* settingsButton = nullptr;
    void OnSettingsButton(wxCommandEvent& evt);

    int openedFrameCount = 0;

    void Init_CreateControls();
    void Init_Layout();
};
