#pragma once
#include "boost/process.hpp"
#include <wx/wx.h>
#include <unordered_map>
#include "TapDevice_SRV.h"
#include "Networks_SRV.h"
#include "tincTextCtrl.h"

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

struct PerTapDataStorage {
    WindowsAPI_SRV::GetAdaptersAddressesResult tap;
    tincTextCtrl* liveLog = nullptr;
    std::shared_ptr<boost::process::child> tincProcess;
    wxButton* connectButton = nullptr;
    wxButton* disconnectButton = nullptr;
};

class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    // UI to SRV
    void API_SRV_ConnectToNetwork(Networks_SRV::GetNetworksResult network, PerTapDataStorage perTapData);

    // SRV to UI
    void API_UI_ReportStatus(std::wstring status, tincTextCtrl* liveLog);
    void API_UI_EndConnectToNetwork(ReturnValue<ConnectToNetworkResult> result, PerTapDataStorage perTapData);

    wxPanel* rootPanel;

    wxStaticText* currentNetwork_StaticText = nullptr;
    wxComboBox* currentNetwork_ComboBox = nullptr;
    std::unordered_map<int, Networks_SRV::GetNetworksResult> currentNetwork_ComboBox_RawData;
    void OnCurrentNetworkChange(wxCommandEvent& evt);

    wxStaticText* currentTap_StaticText = nullptr;
    wxComboBox* currentTap_ComboBox = nullptr;
    std::unordered_map<int, PerTapDataStorage> currentTap_ComboBox_RawData;
    void OnCurrentTapChange(wxCommandEvent& evt);

    wxButton* connectButtonPlaceholder = nullptr;
    void OnConnectButtonClick(wxCommandEvent& evt);
    wxButton* disconnectButtonPlaceholder = nullptr;
    void OnDisconnectButtonClick(wxCommandEvent& evt);

    wxBoxSizer* networkControlSizer = nullptr;

    tincTextCtrl* liveLogPlaceholder = nullptr;
    wxBoxSizer* liveLogSizer = nullptr;

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
