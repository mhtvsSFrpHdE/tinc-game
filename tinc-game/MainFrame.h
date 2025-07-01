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
    std::shared_ptr<wxButton> connectButton;
    std::shared_ptr<wxButton> disconnectButton;
    bool connected = false;
    std::shared_ptr<tincTextCtrl> liveLog;
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
    void ReloadCurrentNetwork();

    wxStaticText* currentTap_StaticText = nullptr;
    wxComboBox* currentTap_ComboBox = nullptr;
    std::unordered_map<int, WindowsAPI_SRV::GetAdaptersAddressesResult> currentTap_ComboBox_RawData;
    wxString GetCurrentTapDisplayText(WindowsAPI_SRV::GetAdaptersAddressesResult tap);
    void UpdateCurrentTapItemDisplayText(WindowsAPI_SRV::GetAdaptersAddressesResult tap, int insertAt);
    std::vector<int> autoStartNetworkRawDataIndex_pending;
    std::vector<int> autoStartNetworkRawDataIndex_submitted;
    void ReloadCurrentTap();

    std::shared_ptr<wxButton> recentActiveConnectButton;
    std::shared_ptr<wxButton> GetInitPhaseDummyConnectButton();
    void OnConnectButtonClick_Internal();
    void OnConnectButtonClick(wxCommandEvent& evt);
    std::shared_ptr<wxButton> recentActiveDisconnectButton;
    std::shared_ptr<wxButton> GetInitPhaseDummyDisconnectButton();
    void OnDisconnectButtonClick(wxCommandEvent& evt);
    std::shared_ptr<tincTextCtrl> recentActiveLiveLog;
    std::shared_ptr<tincTextCtrl> GetInitPhaseDummyLiveLog();

    wxBoxSizer* networkControlSizer = nullptr;
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

    wxMenuBar* menuBar = nullptr;

    wxMenu* networksMenu = nullptr;
    const int wxIdMenuNetworksEdit = wxWindow::NewControlId();
    void OnMenuNetworksEdit(wxCommandEvent& event);
    const int wxIdMenuNetworksJoin = wxWindow::NewControlId();
    void OnMenuNetworksJoin(wxCommandEvent& event);
    void OnJoinNetworkFrameCloseCallback();
    const int wxIdMenuNetworksRename = wxWindow::NewControlId();
    void OnMenuNetworksRename(wxCommandEvent& event);
    void OnRenameNetworkFrameCloseCallback();
    const int wxIdMenuNetworksImportAndExport = wxWindow::NewControlId();
    void OnMenuNetworksImportAndExport(wxCommandEvent& event);
    const int wxIdMenuNetworksReload = wxWindow::NewControlId();
    void OnMenuNetworksReload_Internal();
    void OnMenuNetworksReload(wxCommandEvent& event);
    bool AllowMakeChange(bool showDialog = true);

    wxMenu* networksAdvancedMenu = nullptr;
    const int wxIdMenuNetworksAdvancedDelete = wxWindow::NewControlId();
    void OnMenuNetworksAdvancedDelete(wxCommandEvent& event);

    wxMenu* toolsMenu = nullptr;
    const int wxIdMenuToolsManageTap = wxWindow::NewControlId();
    void OnMenuToolsManageTap(wxCommandEvent& event);
    void OnManageTapFrameCloseCallback();

    void Init_CreateControls();
    void Init_Layout();
    void Init_PostLayout();

    bool allowCloseFrame = true;
    void OnClose(wxCloseEvent& event);
};
