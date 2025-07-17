#pragma once
#include <boost/process.hpp>
#include <wx/wx.h>
#include <unordered_map>
#include "TapDevice_SRV.h"
#include "Networks_SRV.h"
#include "tincTextCtrl.h"
#include "boost/optional.hpp"
#include "..\resource\ReturnValue.h"

struct ConnectToNetworkResult {
    enum class Enum {
        TapUnavailable,
        RefusedByTinc,
        TincNotExist,
        TapNotFound,
        Other
    };
    Enum messageEnum = Enum::Other;
    std::wstring messageString;
    Networks_SRV::GetNetworksResult network;
    WindowsAPI_SRV::GetAdaptersAddressesResult tap;
};

struct PerNetworkData {
    static wxFrame* parentFrame;
    Networks_SRV::GetNetworksResult network;
    WindowsAPI_SRV::GetAdaptersAddressesResult* tap = nullptr;
    int tapSelection = wxNOT_FOUND;
    std::shared_ptr<boost::process::child> tincProcess;
    std::shared_ptr<wxButton> connectButton;
    std::shared_ptr<wxButton> disconnectButton;
    std::shared_ptr<wxTextCtrl> ipTextCtrl;
    std::shared_ptr<wxButton> ipCopyAndRefreshButton;
    bool connected = false;
    std::shared_ptr<tincTextCtrl> liveLog;

    PerNetworkData() {};
    PerNetworkData(const PerNetworkData&) = delete; // Delete copy constructor
    PerNetworkData& operator=(const PerNetworkData&) = delete; // Delete copy assignment operator

    bool requestIpReportThreadStop = false;
    void IpReportThread();
    std::shared_ptr<std::thread> ipReportThread;
};

class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    std::mutex uiMutex;
    std::condition_variable uiCb;

    void API_SRV_ConnectToNetwork(PerNetworkData* perNetworkData);
    ReturnValue<std::wstring> API_SRV_DisconnectNetwork(PerNetworkData* perNetworkData);
    void API_SRV_PostLayout();

    void API_UI_SetDisconnectStatus(bool enable, PerNetworkData* perNetworkData);
    void API_UI_SetEditButtonEnable(bool enable, wxButton* editButton);
    void API_UI_ReportStatus(std::wstring status, tincTextCtrl* liveLog);
    void API_UI_EndConnectToNetwork(ReturnValue<ConnectToNetworkResult> result, PerNetworkData* perNetworkData);

    wxPanel* rootPanel;

    wxStaticText* currentNetwork_StaticText;
    wxComboBox* currentNetwork_ComboBox;
    std::unordered_map<int, std::shared_ptr<PerNetworkData>> currentNetwork_ComboBox_RawData;
    void OnCurrentNetworkChange(wxCommandEvent& evt);
    int recentUsedNetworkSelection = wxNOT_FOUND;
    void ReloadCurrentNetwork();

    wxStaticText* currentTap_StaticText;
    wxComboBox* currentTap_ComboBox;
    std::unordered_map<int, WindowsAPI_SRV::GetAdaptersAddressesResult> currentTap_ComboBox_RawData;
    wxString GetCurrentTapDisplayText(WindowsAPI_SRV::GetAdaptersAddressesResult tap);
    void UpdateCurrentTapItemDisplayText(WindowsAPI_SRV::GetAdaptersAddressesResult tap, int insertAt);
    std::vector<int> autoStartNetworkRawDataIndex_pending;
    std::vector<int> autoStartNetworkRawDataIndex_submitted;
    void ReloadCurrentTap();

    wxBoxSizer* networkControlNavigateSizer;
    std::shared_ptr<wxButton> recentActiveConnectButton;
    std::shared_ptr<wxButton> GetInitPhaseDummyConnectButton();
    void OnConnectButtonClick_Internal();
    void OnConnectButtonClick(wxCommandEvent& evt);
    void OnNetworkConnected(PerNetworkData* perNetworkData);
    std::shared_ptr<wxButton> recentActiveDisconnectButton;
    std::shared_ptr<wxButton> GetInitPhaseDummyDisconnectButton();
    void OnDisconnectButtonClick(wxCommandEvent& evt);
    void OnNetworkDisconnected(PerNetworkData* perNetworkData);

    wxBoxSizer* networkControlIpSizer;
    wxStaticText* ipStaticText;
    std::shared_ptr<wxTextCtrl> recentActiveIpTextCtrl;
    std::shared_ptr<wxTextCtrl> GetInitPhaseDummyIpTextCtrl();
    std::shared_ptr<wxButton> recentActiveIpCopyAndRefreshButton;
    std::shared_ptr<wxButton> GetInitPhaseDummyIpCopyAndRefreshButton();
    void OnIpCopyAndRefreshButtonClick(wxCommandEvent& evt);
    std::shared_ptr<tincTextCtrl> recentActiveLiveLog;
    std::shared_ptr<tincTextCtrl> GetInitPhaseDummyLiveLog();

    wxBoxSizer* liveLogSizer;

    wxMenuBar* menuBar;

    wxMenu* networksMenu;
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

    wxMenu* networksAdvancedMenu;
    const int wxIdMenuNetworksAdvancedDelete = wxWindow::NewControlId();
    void OnMenuNetworksAdvancedDelete(wxCommandEvent& event);

    wxMenu* toolsMenu;
    const int wxIdMenuToolsManageTap = wxWindow::NewControlId();
    void OnMenuToolsManageTap(wxCommandEvent& event);
    void OnManageTapFrameCloseCallback();
    const int wxIdMenuToolsSettings = wxWindow::NewControlId();
    void OnMenuToolsSettings(wxCommandEvent& event);
    int openedFrameCount = 0;

    wxMenu* toolsAdvancedMenu;
    const int wxIdMenuToolsAdvancedOptimizeMtu = wxWindow::NewControlId();
    void OnMenuToolsAdvancedOptimizeMtu(wxCommandEvent& event);
    void OnOptimizeMtuFrameCloseCallback();
    const int wxIdMenuToolsAdvancedTroubleshoot = wxWindow::NewControlId();
    void OnMenuToolsAdvancedTroubleshoot(wxCommandEvent& event);
    void OnTroubleshootFrameCloseCallback();

    void Init_CreateControls();
    void Init_Layout();
    void Init_PostLayout();

    bool allowCloseFrame = true;
    void OnClose(wxCloseEvent& event);
};
