#pragma once
#include "boost/process.hpp"
#include <wx/wx.h>
#include <unordered_map>
#include "TapDevice_SRV.h"
#include "Networks_SRV.h"
#include "tincTextCtrl.h"
#include "boost/optional.hpp"
#include "ReturnValue.h"

struct ConnectToNetworkResult {
    enum class Enum {
        TapUnavailable,
        RefusedByTinc,
        TincNotExist,
        Other
    };
    Enum messageEnum = Enum::Other;
    std::wstring messageString;
    Networks_SRV::GetNetworksResult network;
    WindowsAPI_SRV::GetAdaptersAddressesResult tap;
};

struct PerNetworkData {
    Networks_SRV::GetNetworksResult network;
    WindowsAPI_SRV::GetAdaptersAddressesResult* tap = nullptr;
    int tapSelection = wxNOT_FOUND;
    std::shared_ptr<boost::process::child> tincProcess;
    wxButton* connectButton = nullptr;
    wxButton* disconnectButton = nullptr;
    wxButton* editButton = nullptr;
    wxButton* joinNetworkButton = nullptr;
    tincTextCtrl* liveLog = nullptr;
};

class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    std::mutex uiMutex;
    std::condition_variable uiCb;

    // UI to SRV
    void API_SRV_ConnectToNetwork(PerNetworkData* perNetworkData);
    ReturnValue<std::wstring> API_SRV_DisconnectNetwork(PerNetworkData* perNetworkData);
    void API_SRV_PostLayout();

    // SRV to UI
    void API_UI_SetDisconnectButtonEnable(bool enable, wxButton* disconnectButton);
    void API_UI_SetEditButtonEnable(bool enable, wxButton* editButton);
    void API_UI_ReportStatus(std::wstring status, tincTextCtrl* liveLog);
    void API_UI_EndConnectToNetwork(ReturnValue<ConnectToNetworkResult> result, PerNetworkData* perNetworkData);

    wxPanel* rootPanel;

    wxStaticText* currentNetwork_StaticText = nullptr;
    wxComboBox* currentNetwork_ComboBox = nullptr;
    std::unordered_map<int, PerNetworkData> currentNetwork_ComboBox_RawData;
    void OnCurrentNetworkChange(wxCommandEvent& evt);
    int recentUsedNetworkSelection = wxNOT_FOUND;

    wxStaticText* currentTap_StaticText = nullptr;
    wxComboBox* currentTap_ComboBox = nullptr;
    std::unordered_map<int, WindowsAPI_SRV::GetAdaptersAddressesResult> currentTap_ComboBox_RawData;
    wxString GetCurrentTapDisplayText(WindowsAPI_SRV::GetAdaptersAddressesResult tap);
    void UpdateCurrentTapItemDisplayText(WindowsAPI_SRV::GetAdaptersAddressesResult tap, int insertAt);
    std::vector<int> autoStartNetworkRawDataIndex_pending;
    std::vector<int> autoStartNetworkRawDataIndex_submitted;

    wxButton* connectButtonPlaceholder = nullptr;
    void OnConnectButtonClick_Internal();
    void OnConnectButtonClick(wxCommandEvent& evt);
    wxButton* disconnectButtonPlaceholder = nullptr;
    void OnDisconnectButtonClick(wxCommandEvent& evt);
    wxButton* editButtonPlaceholder = nullptr;
    void OnEditButtonClick(wxCommandEvent& evt);
    wxButton* joinNetworkButtonPlaceholder = nullptr;
    void OnJoinNetworkButtonClick(wxCommandEvent& evt);

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
    void Init_PostLayout();
};
