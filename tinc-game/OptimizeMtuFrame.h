#pragma once
#include <wx/wx.h>
#include <thread>
#include "ReturnValue.h"
#include "MainFrame.h"

struct CheckAddressFormatResult {
    enum class Enum {
        Other,
        NotIPv4,
        NotDomain
    };
};

class OptimizeMtuFrame : public wxFrame
{
public:
    OptimizeMtuFrame(MainFrame* parentFrame);

    // UI to SRV
    static ReturnValue<CheckAddressFormatResult::Enum> API_SRV_CheckAddressFormat(std::wstring ipAddress);
    void API_SRV_StartMeasureMTU(std::wstring ipAddress);

    // SRV to UI
    void API_UI_ReportStatus(std::wstring status);
    void API_UI_ReportMTU_IPv4(int mtu);
    void API_UI_ReportMTU_IPv6(int mtu);
    void API_UI_EndMeasureMTU(bool success, std::wstring reason);

    wxButton* helpButton = nullptr;
private:
    MainFrame* _parentFrame = nullptr;

    wxPanel* rootPanel = nullptr;

    wxStaticText* chooseAddress_StaticText = nullptr;
    wxComboBox* chooseAddress_ComboBox = nullptr;
    wxButton* startButton = nullptr;
    wxButton* applyButton = nullptr;
    wxButton* closeButton = nullptr;
    wxStaticText* mtu_IPv4_StaticText = nullptr;
    wxStaticText* mtuValue_IPv4_StaticText = nullptr;
    wxStaticText* mtu_IPv6_StaticText = nullptr;
    wxStaticText* mtuValue_IPv6_StaticText = nullptr;
    const wxString mtuValue_DefaultText = _("Waiting for value...");
    wxTextCtrl* liveLog = nullptr;

    void Init_CreateControls();
    void Init_Layout();

    void OnClose(wxCloseEvent& event);
    void OnHelpButtonClick(wxCommandEvent& event);
    void OnHelpFrameCloseCallback();
    void OnStartButtonClick(wxCommandEvent& event);
    void OnApplyButtonClick(wxCommandEvent& event);
    void OnCloseButtonClick(wxCommandEvent& event);

    void OpenApplyMtuFrame();

    int mtuValue_IPv4 = 0;
    int reportMtuCount_IPv4 = 0;
    int mtuValue_IPv6 = 0;
    int reportMtuCount_IPv6 = 0;

    bool allowCloseFrame = true;
};

class OptimizeMtuFrameTest {
public:
    static void TestCheckAddressFormat(wxWindow* parent);
};
