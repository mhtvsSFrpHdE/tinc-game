#pragma once
#include <wx/wx.h>
#include <thread>
#include "..\resource\ReturnValue.h"

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
    OptimizeMtuFrame(wxFrame* parentFrame, std::function<void()> onCloseCallback);

    static ReturnValue<CheckAddressFormatResult::Enum> API_SRV_CheckAddressFormat(std::wstring ipAddress);
    void API_SRV_StartMeasureMTU(std::wstring ipAddress);

    void API_UI_ReportStatus(std::wstring status);
    void API_UI_ReportMTU_IPv4(int mtu);
    void API_UI_ReportMTU_IPv6(int mtu);
    void API_UI_EndMeasureMTU(bool success, std::wstring reason);

    wxButton* helpButton;
private:
    wxFrame* _parentFrame;
    std::function<void()> _onCloseCallback;

    wxPanel* rootPanel;

    wxStaticText* chooseAddress_StaticText;
    wxComboBox* chooseAddress_ComboBox;
    wxButton* startButton;
    wxButton* applyButton;
    wxButton* closeButton;
    wxStaticText* mtu_IPv4_StaticText;
    wxStaticText* mtuValue_IPv4_StaticText;
    wxStaticText* mtu_IPv6_StaticText;
    wxStaticText* mtuValue_IPv6_StaticText;
    const wxString mtuValue_DefaultText = _("Waiting for value...");
    wxTextCtrl* liveLog;

    void Init_CreateControls();
    void Init_Layout();

    bool allowCloseFrame = true;
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
};

class OptimizeMtuFrameTest {
public:
    static void TestCheckAddressFormat(wxWindow* parent);
};
